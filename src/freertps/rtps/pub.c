// Copyright 2016 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "freertps/freertps.h"
#include "freertps/utility.h"
#include "freertps/rtps/type/config.h"
#include "freertps/rtps/publisher.h"
#include "freertps/rtps/writer.h"
#include "freertps/rtps/constant/sub_message_flags.h"
#include "freertps/rtps/constant/sub_message_id.h"
#include "freertps/rtps/type/part.h"
#include "freertps/rtps/type/sub_message_heartbeat.h"
#include "freertps/rtps/discovery/sedp.h"
#include "freertps/rtps/discovery/spdp.h"
#include "freertps/psm/bswap.h"
#include "freertps/psm/udp.h"

#include <stdio.h>
#include <string.h>

frudp_writer_t g_frudp_writers[FRUDP_MAX_WRITERS];
uint32_t g_frudp_num_writers = 0;

frudp_pub_t g_frudp_pubs[FRUDP_MAX_PUBS];
uint32_t g_frudp_num_pubs = 0;

static uint8_t g_pub_tx_buf[FRUDP_PUB_BUFLEN];
//static uint8_t g_pub_user_tx_buf[FRUDP_PUB_BUFLEN];

///////////////////////////////////////////////////////////////////////////

frudp_pub_t *frudp_create_pub(const char *topic_name,
                              const char *type_name,
                              frudp_qos_reliability_t *qos,
                              const frudp_eid_t writer_id,
                              frudp_submsg_data_t **data_submsgs,
                              const uint32_t num_data_submsgs)
{
  FREERTPS_INFO("Create pub 0x%08x\r\n", freertps_htonl(writer_id.u));

  if (g_frudp_num_pubs >= FRUDP_MAX_PUBS)
  {
    FREERTPS_ERROR("woah there partner. don't have space for more pubs.\r\n");
    return NULL; // no room. sorry
  }

  frudp_pub_t *p = &g_frudp_pubs[g_frudp_num_pubs];
  p->topic_name = topic_name;
  p->type_name = type_name;

  //TODO fix by full structure
  p->qos.kind = FRUDP_QOS_RELIABILITY_KIND_RELIABLE;
  if (qos != NULL) {
	  p->qos.kind = qos->kind;
  }

  if (data_submsgs) // it's for a reliable connection
  {
    p->data_submsgs = data_submsgs;
    p->num_data_submsgs = num_data_submsgs;
    p->reliable = true;
  }
  else
  {
    // unreliable; fire-and-forget from the caller's memory
    p->data_submsgs = NULL;
    p->num_data_submsgs = 0;
    p->reliable = false;
  }

  if (writer_id.u == g_frudp_eid_unknown.u)
  {
    FREERTPS_ERROR("AWAY WITH YOU! <point scepter>\r\n");
    return NULL;
  }
  else
  {
    p->writer_eid.u = writer_id.u;
  }

  p->next_submsg_idx = 0;
  p->next_sn.low = 1;
  p->next_sn.high = 0;
  g_frudp_num_pubs++;

  return p;
}

frudp_pub_t *frudp_create_user_pub(const char *topic_name,
                                   const char *type_name,
                                   frudp_qos_reliability_t *qos)
{
  FREERTPS_DEBUG("create_user_pub(%s, %s)\r\n", topic_name, type_name);
  frudp_pub_t *pub = frudp_create_pub(topic_name,
                                      type_name,
                                      qos,
                                      frudp_create_user_id
                                      (FRUDP_ENTITY_KIND_USER_WRITER_NO_KEY),
                                      NULL,
                                      0);
  //sedp_publish_pub(pub); // can't do this yet; disco hasn't started
  return pub;
}

void frudp_send_sedp_msgs(frudp_part_t *part)
{
  frudp_spdp_bcast(part);
  // we have just found out about a new participant. blast our SEDP messages
  // at it to help it join quickly

  // first, send the publications
  if (g_sedp_pub_pub->next_submsg_idx)
  {
    _SEDP_INFO("\tsending %d SEDP publication catchup messages\r\n", g_sedp_pub_pub->next_submsg_idx);
    frudp_msg_t *msg = frudp_init_msg((frudp_msg_t *)g_pub_tx_buf);
    fr_time_t t = fr_time_now();
    uint16_t submsg_wpos = 0;

    frudp_submsg_t *ts_submsg = (frudp_submsg_t *)&msg->submsgs[submsg_wpos];
    ts_submsg->header.id = FRUDP_SUBMSG_ID_INFO_TS;
    ts_submsg->header.flags = FRUDP_FLAGS_LITTLE_ENDIAN;
    ts_submsg->header.len = 8;
    memcpy(ts_submsg->contents, &t, 8);
    submsg_wpos += 4 + 8;

    for (int i = 0; i < g_sedp_pub_pub->next_submsg_idx; i++)
    {
      // todo: make sure we don't overflow a single ethernet frame
      // since that would be most non-triumphant
      frudp_submsg_data_t *pub_submsg = g_sedp_pub_pub->data_submsgs[i];
      memcpy(&msg->submsgs[submsg_wpos], pub_submsg,
          4 + pub_submsg->header.len);
      submsg_wpos += 4 + pub_submsg->header.len;
      _SEDP_INFO("\tcatchup SEDP msg %d addressed to reader EID 0x%08x\r\n",
          i, freertps_htonl((unsigned)pub_submsg->reader_id.u));
    }
    frudp_submsg_heartbeat_t *hb_submsg =
      (frudp_submsg_heartbeat_t *)&msg->submsgs[submsg_wpos];
    hb_submsg->header.id = FRUDP_SUBMSG_ID_HEARTBEAT;
    hb_submsg->header.flags = 0x3; // todo: spell this out
    hb_submsg->header.len = 28;
    hb_submsg->reader_id = g_sedp_pub_pub->data_submsgs[0]->reader_id;
    hb_submsg->writer_id = g_sedp_pub_pub->data_submsgs[0]->writer_id;
    hb_submsg->first_sn.low = 1; // todo
    hb_submsg->first_sn.high = 0; // todo
    hb_submsg->last_sn.low = g_sedp_pub_pub->next_submsg_idx;
    hb_submsg->last_sn.high = 0; // todo
    hb_submsg->count = 0;
    submsg_wpos += 4 + hb_submsg->header.len;
    int payload_len = &msg->submsgs[submsg_wpos] - ((uint8_t *)msg);
    uint32_t dst_addr = freertps_htonl(part->metatraffic_unicast_locator.address.udp4.address);
    uint16_t dst_port = part->metatraffic_unicast_locator.port;
    _SEDP_INFO("\tsending %d bytes of SEDP pub catchup messages to %s:%d\r\n",
           payload_len, frudp_print_ip(dst_addr), dst_port);

    frudp_tx(dst_addr, dst_port, (const uint8_t *)msg, payload_len);
  }
  else
    _SEDP_WARNING("\tno SEDP Publisher Publisher data to send to new participant\r\n");

  // now, send the subscriptions
  if (g_sedp_sub_pub->next_submsg_idx)
  {
    _SEDP_INFO("\tsending %d SEDP subscription catchup messages\r\n",
           g_sedp_sub_pub->next_submsg_idx);
    frudp_msg_t *msg = frudp_init_msg((frudp_msg_t *)g_pub_tx_buf);
    fr_time_t t = fr_time_now();
    uint16_t submsg_wpos = 0;

    frudp_submsg_t *ts_submsg = (frudp_submsg_t *)&msg->submsgs[submsg_wpos];
    ts_submsg->header.id = FRUDP_SUBMSG_ID_INFO_TS;
    ts_submsg->header.flags = FRUDP_FLAGS_LITTLE_ENDIAN;
    ts_submsg->header.len = 8;
    memcpy(ts_submsg->contents, &t, 8);
    submsg_wpos += 4 + 8;

    for (int i = 0; i < g_sedp_sub_pub->next_submsg_idx; i++)
    {
      // todo: make sure we don't overflow a single ethernet frame,
      // since that would be most non-triumphant
      frudp_submsg_data_t *sub_submsg = g_sedp_sub_pub->data_submsgs[i];
      memcpy(&msg->submsgs[submsg_wpos], sub_submsg,
          4 + sub_submsg->header.len);
      submsg_wpos += 4 + sub_submsg->header.len;
      _SEDP_INFO("\tcatchup SEDP msg %d addressed to reader EID 0x%08x\r\n",
          i, freertps_htonl((unsigned)sub_submsg->reader_id.u));
    }
    frudp_submsg_heartbeat_t *hb_submsg =
      (frudp_submsg_heartbeat_t *)&msg->submsgs[submsg_wpos];
    hb_submsg->header.id = FRUDP_SUBMSG_ID_HEARTBEAT;
    hb_submsg->header.flags = 0x3; // todo: spell this out
    hb_submsg->header.len = 28;
    hb_submsg->reader_id = g_sedp_sub_pub->data_submsgs[0]->reader_id;
    hb_submsg->writer_id = g_sedp_sub_pub->data_submsgs[0]->writer_id;
    hb_submsg->first_sn.low = 1; // todo
    hb_submsg->first_sn.high = 0; // todo
    hb_submsg->last_sn.low = g_sedp_sub_pub->next_submsg_idx;
    hb_submsg->last_sn.high = 0; // todo
    hb_submsg->count = 0;
    submsg_wpos += 4 + hb_submsg->header.len;
    int payload_len = &msg->submsgs[submsg_wpos] - ((uint8_t *)msg);
    uint32_t dst_addr = part->metatraffic_unicast_locator.address.udp4.address;
    uint16_t dst_port = part->metatraffic_unicast_locator.port;
    _SEDP_INFO("\tsending %d bytes of SEDP sub catchup messages to %s:%d\r\n",
           payload_len, frudp_print_ip(freertps_htonl(dst_addr)), dst_port);

    frudp_tx(freertps_htonl(dst_addr), dst_port, (const uint8_t *)msg, payload_len);
  }
  else
    _SEDP_WARNING("\tno SEDP Subscription Publisher data to send to new participant\r\n");
}

// currently this only gets called for SEDP messages
void frudp_publish(frudp_pub_t *pub, frudp_submsg_data_t *submsg)
{
  // TODO: for best-effort connections, don't buffer it like this
  // check to see if the publisher has a buffer or not, first....

  // (todo: allow people to stuff the message directly in the pub and
  // call this function with sample set to NULL to indicate this)

  // find first place we can buffer this sample
  pub->max_tx_sn_avail.low++; // TODO: care about sample counts over 32 bits
  frudp_submsg_data_t *pub_submsg = pub->data_submsgs[pub->next_submsg_idx];

  *pub_submsg = *submsg;
  if (submsg->writer_sn.low == g_frudp_sn_unknown.low) // todo: 64 bits
  {
    pub_submsg->writer_sn = pub->next_sn;
    pub->next_sn.low++; // todo: > 32 bits
  }
  memcpy(pub_submsg->data,
         submsg->data,
         submsg->header.len - sizeof(frudp_submsg_data_t) + 4);
  //pub_sample->data_len = sample->data_len;
  // TODO: now, send DATA and HEARTBEAT submessages
  FREERTPS_INFO("frudp publish %d bytes, seq num %d:%d\r\n",
         submsg->header.len,
         (int)pub_submsg->writer_sn.high,
         (int)pub_submsg->writer_sn.low);
/*
  /////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////
  //data_submsg->header.len = next_submsg_ptr - data_submsg->contents;
  //FREERTPS_INFO("len = %d\n", data_submsg->header.len);
  /////////////////////////////////////////////////////////////
  //int payload_len = ((uint8_t *)param_list) - ((uint8_t *)msg->submsgs);
  //int payload_len = ((uint8_t *)next_submsg_ptr) - ((uint8_t *)msg->submsgs);
  int payload_len = ((uint8_t *)next_submsg_ptr) - ((uint8_t *)msg);
  frudp_tx(inet_addr("239.255.0.1"), frudp_mcast_builtin_port(),
           (const uint8_t *)msg, payload_len);
*/
  frudp_msg_t *msg = frudp_init_msg((frudp_msg_t *)g_pub_tx_buf);
  fr_time_t t = fr_time_now();
  uint16_t submsg_wpos = 0;
  
  frudp_submsg_t *ts_submsg = (frudp_submsg_t *)&msg->submsgs[submsg_wpos];
  ts_submsg->header.id = FRUDP_SUBMSG_ID_INFO_TS;
  ts_submsg->header.flags = FRUDP_FLAGS_LITTLE_ENDIAN;
  ts_submsg->header.len = 8;
  memcpy(ts_submsg->contents, &t, 8);
  submsg_wpos += 4 + 8;
  
  ///////////////////////////////////////////////////////////////////////
  //frudp_submsg_data_t *data_submsg = (frudp_submsg_data_t *)&msg->submsgs[submsg_wpos];
  memcpy(&msg->submsgs[submsg_wpos], submsg, 4 + submsg->header.len);
  //data_submsg, submsg, 4 + submsg->header.len);
  submsg_wpos += 4 + submsg->header.len;

  frudp_submsg_heartbeat_t *hb_submsg =
    (frudp_submsg_heartbeat_t *)&msg->submsgs[submsg_wpos];
  hb_submsg->header.id = FRUDP_SUBMSG_ID_HEARTBEAT;
  hb_submsg->header.flags = 0x3; // todo: spell this out
  hb_submsg->header.len = 28;
  hb_submsg->reader_id = submsg->reader_id;
  hb_submsg->writer_id = submsg->writer_id;
  hb_submsg->first_sn.low = 1; // todo, should increase each time (?)
  hb_submsg->first_sn.high = 0; // todo
  hb_submsg->last_sn = hb_submsg->first_sn; //submsg->writer_sn;
  /*
  FREERTPS_INFO(" hb submsg publish last_sn = %d:%d\n",
         (int)hb_submsg->last_sn.high,
         (int)hb_submsg->last_sn.low);
  */
  static int hb_count = 0;
  hb_submsg->count = hb_count++;

  submsg_wpos += 4 + hb_submsg->header.len;

  int payload_len = &msg->submsgs[submsg_wpos] - ((uint8_t *)msg);
  frudp_tx(FRUDP_DEFAULT_MCAST_GROUP,
           frudp_mcast_builtin_port(),
           (const uint8_t *)msg, payload_len);
  pub->next_submsg_idx++;

  /////////////////////////////////////////////////////////////
  /*
  ts_submsg = (frudp_submsg_t *)param_list;
  ts_submsg->header.id = FRUDP_SUBMSG_ID_INFO_TS;
  ts_submsg->header.flags = FRUDP_FLAGS_LITTLE_ENDIAN;
  ts_submsg->header.len = 8;
  memcpy(ts_submsg->contents, &t, 8);
  uint8_t *next_submsg_ptr = ((uint8_t *)param_list) + 4 + 8;
  */

  /////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////
  //data_submsg->header.len = next_submsg_ptr - data_submsg->contents;
  //FREERTPS_INFO("len = %d\n", data_submsg->header.len);
  /////////////////////////////////////////////////////////////
  //int payload_len = ((uint8_t *)param_list) - ((uint8_t *)msg->submsgs);
  //int payload_len = ((uint8_t *)next_submsg_ptr) - ((uint8_t *)msg->submsgs);
}

frudp_pub_t *frudp_pub_from_writer_id(const frudp_eid_t id)
{
  //FREERTPS_INFO("pub from writer id 0x%08x\n", htonl(id.u));
  for (int i = 0; i < g_frudp_num_pubs; i++)
  {
    frudp_pub_t *p = &g_frudp_pubs[i];
    //FREERTPS_INFO("  comp %d: 0x%08x ?= 0x%08x\n",
    //       i, htonl(id.u), htonl(p->writer_id.u));
    if (id.u == p->writer_eid.u)
      return p;
  }
  return NULL;
}

void frudp_pub_rx_acknack(frudp_pub_t *pub,
                                frudp_submsg_acknack_t *acknack,
                                frudp_guid_prefix_t *guid_prefix)
{
  // see if we have any of the requested messages
  for (int req_seq_num = acknack->reader_sn_state.bitmap_base.low;
       req_seq_num < acknack->reader_sn_state.bitmap_base.low +
                     acknack->reader_sn_state.num_bits + 1;
       req_seq_num++)
  {
    FREERTPS_INFO("     request for seq num %d\r\n", req_seq_num);
    for (int msg_idx = 0; msg_idx < pub->num_data_submsgs; msg_idx += pub->num_data_submsgs ) //msg_idx++) //TODO Limit n send !!
    {
      frudp_submsg_data_t *data = pub->data_submsgs[msg_idx];
      FREERTPS_INFO("       %d ?= %d\r\n", req_seq_num, data->writer_sn.low);
      if (data->writer_sn.low == req_seq_num)
      {
        FREERTPS_INFO("        found it in the history cache! now i need to tx\r\n");
        FREERTPS_INFO("        about to look up guid prefix: %s\r\n", frudp_print_guid_prefix(guid_prefix));

        // look up the GUID prefix of this reader, so we can call them back
        frudp_part_t *part = frudp_part_find(guid_prefix);
        if (!part)
        {
          FREERTPS_INFO("      woah there partner. you from around these parts?\r\n");
          return;
        }

        frudp_msg_t *msg = frudp_init_msg((frudp_msg_t *)g_pub_tx_buf);
        uint16_t submsg_wpos = 0;
        

		frudp_submsg_t *dst_submsg = (frudp_submsg_t *)&msg->submsgs[submsg_wpos];
		dst_submsg->header.id = FRUDP_SUBMSG_ID_INFO_DEST;
		dst_submsg->header.flags = FRUDP_FLAGS_LITTLE_ENDIAN;
		dst_submsg->header.len = FRUDP_GUID_PREFIX_LEN;
		memcpy(dst_submsg->contents, guid_prefix, FRUDP_GUID_PREFIX_LEN);
		submsg_wpos += 4 + FRUDP_GUID_PREFIX_LEN;

        ///////////////////////////////////////////////////////////////////////
        //frudp_submsg_data_t *data_submsg = (frudp_submsg_data_t *)&msg->submsgs[submsg_wpos];
        memcpy(&msg->submsgs[submsg_wpos], data, 4 + data->header.len);
        //data_submsg, submsg, 4 + submsg->header.len);
        submsg_wpos += data->header.len;

        frudp_submsg_heartbeat_t *hb_submsg = (frudp_submsg_heartbeat_t *)&msg->submsgs[submsg_wpos];
        hb_submsg->header.id = FRUDP_SUBMSG_ID_HEARTBEAT;
        hb_submsg->header.flags = 0x1; //0x3; // todo: spell this out
        hb_submsg->header.len = 28;
        hb_submsg->reader_id = acknack->reader_id; //data->reader_id;
        hb_submsg->writer_id = acknack->writer_id; //data->writer_id;
        //FREERTPS_INFO("hb writer id = 0x%08x\n", htonl(data->writer_id.u));
        hb_submsg->first_sn.low = g_sedp_pub_pub->next_submsg_idx;
        hb_submsg->first_sn.high = 0; // todo
        hb_submsg->last_sn = acknack->reader_sn_state.bitmap_base;
        static int hb_count = 0;
        hb_submsg->count = hb_count++;

        submsg_wpos += 4 + hb_submsg->header.len;

        int payload_len = &msg->submsgs[submsg_wpos] - ((uint8_t *)msg);
        //FREERTPS_INFO("         sending %d bytes\n", payload_len);
        frudp_tx(freertps_htonl(part->metatraffic_unicast_locator.address.udp4.address),
                 part->metatraffic_unicast_locator.port,
                 (const uint8_t *)msg, payload_len);

        // we have to send an ACKNACK now
//        frudp_sn_set_32bits_t set;
        // todo: handle 64-bit sequence numbers
//        set.bitmap_base.high = 0;
//        set.bitmap_base.low = 1;
//
//        if (match->max_rx_sn.low >= hb->last_sn.low) // we're up-to-date
//        {
//          set.bitmap_base.low = hb->first_sn.low + 1;
//          set.num_bits = 0;
//        } else {
//          set.bitmap_base.low = match->max_rx_sn.low + 1;
//          set.num_bits = hb->last_sn.low - match->max_rx_sn.low - 1;
//          if (set.num_bits > 31)
//            set.num_bits = 31;
//        }
//        set.num_bits = 0;
//        set.bitmap = 0xffffffff;

        //frudp_tx_acknack(guid_prefix, &data->reader_id, &data->writer_id, &set);
      }
    }
  }
}

void frudp_add_writer(const frudp_writer_t *writer)
{
  FREERTPS_DEBUG("add_writer(%08x => %s)\r\n",
                 (unsigned)freertps_htonl(writer->writer_eid.u),
                 frudp_print_guid(&writer->reader_guid));

  if (g_frudp_num_writers >= FRUDP_MAX_WRITERS)
    return;

  g_frudp_writers[g_frudp_num_writers] = *writer;
  g_frudp_num_writers++;
}

void frudp_remove_writer(const frudp_writer_t *writer)
{
  // Stop if no writer in memory
  if (g_frudp_num_writers <= 0)
    return;

  // Find removed writer
  int i = 0;
  bool found = false;
  for (; i < g_frudp_num_writers; i++)
  {
    frudp_writer_t *writer_mem = &g_frudp_writers[i];
    if (writer == writer_mem) {
        found = true;
        break;
    }
  }

  // If found remove them.
  if (found)
  {
    g_frudp_writers[i] = g_frudp_writers[g_frudp_num_writers-1];
    g_frudp_num_writers--;
  }

}

bool frudp_publish_user_msg_frag(
    frudp_pub_t *pub,
    const uint32_t frag_num,
    const uint8_t *frag,
    const uint32_t frag_len,
    const uint32_t frag_used_len,
    const uint32_t msg_len)
{
  //FREERTPS_INFO("publish frag %d : %d bytes\n", frag_num, frag_len);
  // todo: consolidate this with the non-fragmented TX function...
  frudp_msg_t *msg = frudp_init_msg((frudp_msg_t *)g_pub_tx_buf); //g_pub_user_tx_buf);
  uint16_t submsg_wpos = 0;

  if (frag_num == 1)
  {
    // craft a tx packet and stuff it
    frudp_submsg_t *ts_submsg = (frudp_submsg_t *)&msg->submsgs[submsg_wpos];
    ts_submsg->header.id = FRUDP_SUBMSG_ID_INFO_TS;
    ts_submsg->header.flags = FRUDP_FLAGS_LITTLE_ENDIAN;
    ts_submsg->header.len = 8;
    fr_time_t t = fr_time_now();
    memcpy(ts_submsg->contents, &t, 8);
    submsg_wpos += 4 + 8;
  }
  // append the data frag submessage ////////////////////////////////////////
  frudp_submsg_data_frag_t *d =
    (frudp_submsg_data_frag_t *)&msg->submsgs[submsg_wpos];
  d->header.id = FRUDP_SUBMSG_ID_DATA_FRAG;
  d->header.flags = FRUDP_FLAGS_LITTLE_ENDIAN;
  d->header.len = sizeof(frudp_submsg_data_frag_t) + frag_used_len;
  d->extraflags = 0;
  d->octets_to_inline_qos = 28;
  d->writer_sn = pub->next_sn;
  if (frag_num == 1)
    pub->next_sn.low++; // todo: something smarter
  d->fragment_starting_number = frag_num;
  d->fragments_in_submessage = 1;
  d->fragment_size = frag_len;
  d->sample_size = msg_len; // + 4;
  /*
  if (frag_num == 1)
  {
    frudp_encapsulation_scheme_t *scheme =
      (frudp_encapsulation_scheme_t *)((uint8_t *)d->data);
    scheme->scheme = freertps_htons(FRUDP_SCHEME_CDR_LE);
    scheme->options = 0;
  }
  */
  uint8_t *outbound_frag_payload = (uint8_t *)&d->data[0];
  memcpy(outbound_frag_payload, frag, frag_used_len);
  submsg_wpos += d->header.len + 4;
  const int udp_payload_len =
    (uint8_t *)&msg->submsgs[submsg_wpos] - (uint8_t *)msg;
  //FREERTPS_INFO("rtps udp payload = %d bytes\n", (int)udp_payload_len);
  // now, iterate through all matched-writers and send the message as needed
  for (int i = 0; i < g_frudp_num_writers; i++)
  {
    frudp_writer_t *w = &g_frudp_writers[i];
    if (w->writer_eid.u == pub->writer_eid.u)
    {
      // we want to send here. if we haven't already sent to the same
      // locator, update the guid and send the message
      // todo: figure out between sending to multicast and sending to unicast
      // and don't re-multicast to the same domain
      d->reader_id = w->reader_guid.eid; // todo copy here...
      d->writer_id = w->writer_eid;
      frudp_part_t *part = frudp_part_find(&w->reader_guid.prefix);
      if (!part)
        continue; // shouldn't happen; this implies inconsistency somewhere
      frudp_tx(freertps_htonl(part->default_unicast_locator.address.udp4.address),
               part->default_unicast_locator.port,
               (const uint8_t *)msg,
               udp_payload_len);
    }
  }
  return true;
}

bool frudp_publish_user_msg(frudp_pub_t *pub,
    const uint8_t *payload, const uint32_t payload_len)
{

#ifdef EXCESSIVELY_VERBOSE_MSG_RX
  FREERTPS_INFO("publish user msg %d bytes on %s => 0x%08x\r\n",
                (int)payload_len,
                pub->topic_name,
                freertps_htonl(pub->writer_eid.u));
#endif

  if (pub->reliable)
  {
    // shouldn't be hard to push this through the reliable-comms machinery
    // written for SEDP, but it's not done yet.
    FREERTPS_ERROR("user reliable publishing not quite done yet.\r\n");
    return false;
  }

  // craft a tx packet and stuff it
  fr_time_t t = fr_time_now();
  uint16_t submsg_wpos = 0;
  frudp_msg_t *msg          = frudp_init_msg((frudp_msg_t *)g_pub_tx_buf); //g_pub_user_tx_buf);

  // append the TS sub-message //////////////////////////////////////////////
  frudp_submsg_t *ts_submsg = (frudp_submsg_t *)&msg->submsgs[submsg_wpos];
  ts_submsg->header.id      = FRUDP_SUBMSG_ID_INFO_TS;
  ts_submsg->header.flags   = FRUDP_FLAGS_LITTLE_ENDIAN;
  ts_submsg->header.len     = 8;
  memcpy(ts_submsg->contents, &t, 8);
  submsg_wpos += 4 + 8;

  // append the data sub-message ///////////////////////////////////////
  frudp_submsg_data_t *data_submsg = (frudp_submsg_data_t *)&msg->submsgs[submsg_wpos];
  data_submsg->header.id      = FRUDP_SUBMSG_ID_DATA;
  data_submsg->header.flags   = FRUDP_FLAGS_LITTLE_ENDIAN |
                                FRUDP_FLAGS_DATA_PRESENT;
  data_submsg->header.len     = sizeof(frudp_submsg_data_t) + payload_len;
  data_submsg->extraflags     = 0;
  data_submsg->writer_sn      = pub->next_sn;
  data_submsg->octets_to_inline_qos = 16;

  frudp_encapsulation_scheme_t *scheme =
    (frudp_encapsulation_scheme_t *)((uint8_t *)data_submsg->data);
  scheme->scheme = freertps_htons(FRUDP_SCHEME_CDR_LE);
  scheme->options = 0;

  uint8_t *outbound_payload = (uint8_t *)(&data_submsg->data[4]);
  memcpy(outbound_payload, payload, payload_len);
  submsg_wpos += 4 + data_submsg->header.len;

  ///////////////////////////////////////////////////////////////////////
  frudp_submsg_heartbeat_t *hb =
    (frudp_submsg_heartbeat_t *)&msg->submsgs[submsg_wpos];
  hb->header.id = FRUDP_SUBMSG_ID_HEARTBEAT;
  hb->header.flags = 0x1; // todo: spell this out
  hb->header.len = 28;
  hb->first_sn = pub->next_sn; // todo
  hb->last_sn.high = 0;
  hb->last_sn.low = 0;
  static int hb_count = 0;
  hb->count = hb_count++;

  submsg_wpos += 4 + hb->header.len;

  const int udp_payload_len =
    (uint8_t *)&msg->submsgs[submsg_wpos] - (uint8_t *)msg;
  //FREERTPS_INFO("rtps udp payload = %d bytes\n", (int)udp_payload_len);

  // now, iterate through all matched-writers and send the message as needed
  if (g_frudp_num_writers > 0)
  {
    for (int i = 0; i < g_frudp_num_writers; i++)
    {
      frudp_writer_t *w = &g_frudp_writers[i];
      if (w->writer_eid.u == pub->writer_eid.u)
      {
        // we want to send here. if we haven't already sent to the same
        // locator, update the guid and send the message
        // todo: figure out between sending to multicast and sending to unicast
        // and don't re-multicast to the same domain
        data_submsg->reader_id = w->reader_guid.eid; // todo copy here...
        data_submsg->writer_id = w->writer_eid;

        frudp_part_t *part = frudp_part_find(&w->reader_guid.prefix);
        if (!part)
          continue; // shouldn't happen; this implies inconsistency somewhere
        // also, update the reader/writer ID's for the heartbeat submsg
        // actually.. i don't think we have to send heartbeats to best-effort..

        hb->reader_id = w->reader_guid.eid;
        hb->writer_id = w->writer_eid;

        frudp_locator_t loc = part->default_unicast_locator;
        frudp_tx(freertps_htonl(loc.address.udp4.address), loc.port,
                 (const uint8_t *)msg, udp_payload_len);

      }
    }
  }
  else
  {
#ifdef EXCESSIVELY_VERBOSE_MSG_RX
  FREERTPS_DEBUG("  publish user msg %d bytes, but no node to send...\r\n", (int)payload_len);
#endif
  }

  pub->next_sn.low++; // todo: 64-bit
  return true;
}

