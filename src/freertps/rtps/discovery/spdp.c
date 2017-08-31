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
#include "freertps/psm/bswap.h"
#include "freertps/psm/udp.h"
#include "freertps/rtps/sub.h"
#include "freertps/rtps/constant/parameter_id.h"
#include "freertps/rtps/constant/submsg_flags.h"
#include "freertps/rtps/constant/vendor.h"
#include "freertps/rtps/discovery/disco.h"
#include "freertps/rtps/discovery/spdp.h"
#include "freertps/rtps/discovery/sedp.h"

#include <string.h>
#include <time.h>
#include <inttypes.h>

////////////////////////////////////////////////////////////////////////////
// local constants
static frudp_part_t g_frudp_spdp_rx_part; // just for rx buffer
static fr_time_t g_frudp_spdp_last_bcast;

const frudp_eid_t g_spdp_writer_id = { .u = 0xc2000100 };
const frudp_eid_t g_spdp_reader_id = { .u = 0xc7000100 };

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

static bool frudp_udp_is_same_network(uint32_t ip, uint32_t netip, uint32_t netmask)
{
#ifdef DEBUG
    char src_ip[16] = {0}, dst_ip[16] = {0};
    memcpy(src_ip, frudp_print_ip(netip), 16);
    memcpy(dst_ip, frudp_print_ip(ip), 16);

    _SPDP_DEBUG("frudp_udp_is_same_network() %s ?? %s / %s\r\n",
            src_ip,
            dst_ip,
            frudp_print_ip(netmask));
#endif
    return ((netip & netmask) == (ip & netmask));
}

static void frudp_spdp_clean()
{
  _SPDP_DEBUG("frudp_spdp_clean()\r\n");
  const fr_time_t t = fr_time_now();
  for (unsigned i = 0; i < g_frudp_disco_num_parts; i++) {
    frudp_part_t *match = &g_frudp_disco_parts[i];
    if (fr_time_diff(&t, &match->last_spdp ).seconds > match->lease_duration.sec)
    {
      g_frudp_disco_parts[i] = g_frudp_disco_parts[g_frudp_disco_num_parts - 1];
      g_frudp_disco_num_parts--;
    }
  }
}

static bool frudp_data_generic(frudp_parameterid_t pid)
{
  bool result = true;

  switch(pid)
  {
    ////////////////////////////////////////////////////////////////////////////
    // Check why this...
    case FRUDP_PID_PROPERTY_LIST:
      _SPDP_INFO("\tSPDP Extra field: \t\t\t(Todo)\r\n");
      break;
    ////////////////////////////////////////////////////////////////////////////
    // Deprecated...
    case FRUDP_PID_PERSISTENCE:
    case FRUDP_PID_TYPE_CHECKSUM:
    case FRUDP_PID_TYPE2_NAME:
    case FRUDP_PID_TYPE2_CHECKSUM:
    case FRUDP_PID_EXPECTS_ACK:
    case FRUDP_PID_MANAGER_KEY:
    case FRUDP_PID_SEND_QUEUE_SIZE:
    case FRUDP_PID_RELIABILITY_ENABLED:
    case FRUDP_PID_VARGAPPS_SEQUENCE_NUMBER_LAST:
    case FRUDP_PID_RECV_QUEUE_SIZE:
    case FRUDP_PID_RELIABILITY_OFFERED:
      _SPDP_INFO("\tSPDP Deprecated field (Todo)!\r\n");
      break;
    ////////////////////////////////////////////////////////////////////////////
    default:
      result = false;
  }

  return result;
}

static void frudp_spdp_rx_data(frudp_receiver_state_t *rcvr,
                               const frudp_submsg_t *submsg,
                               const uint16_t scheme,
                               const uint8_t *data)
{
  frudp_locator_t *loc;
  frudp_duration_t *dur;
  frudp_guid_t *guid;

  _SPDP_DEBUG("frudp_spdp_rx_data()\r\n");
  _SPDP_INFO("    SPDP rx:\r\n");

  if (scheme != FRUDP_SCHEME_PL_CDR_LE)
  {
    _SPDP_ERROR("expected SPDP data to be PL_CDR_LE. bailing...\r\n");
    return;
  }

  frudp_part_t *part = &g_frudp_spdp_rx_part;
  // todo: spin through this param list and save it
  frudp_parameter_list_item_t *item = (frudp_parameter_list_item_t *)data;
  while ((uint8_t *)item < submsg->contents + submsg->header.len)
  {
    const frudp_parameterid_t pid = item->pid;

    if (pid == FRUDP_PID_SENTINEL) { break; }
    if (pid & 0x8000) { /* ignore vendor-specific PID's */ }
    if (pid & 0x4000) { /* ignore ParameterID */ }

    const uint8_t *pval = item->value;

    _SPDP_DEBUG("\tunknown SPDP rx param 0x%x len %d\r\n",
                (unsigned)pid, item->len);

    switch(pid)
    {
    ////////////////////////////////////////////////////////////////////////////
    case FRUDP_PID_PROTOCOL_VERSION:
      part->pver = *((frudp_pver_t *)(pval)); // todo: what about alignment?
      _SPDP_INFO("\tSPDP proto version \t\t\t\t0x%04x (DDS-RTPS)\r\n",
                 part->pver.major);
      break;
    ////////////////////////////////////////////////////////////////////////////
    case FRUDP_PID_VENDOR_ID:
      part->vid = freertps_htons(*((frudp_vid_t *)pval));
      _SPDP_INFO("\tSPDP vendor_id \t\t\t\t\t0x%04x = %s\r\n",
                 part->vid, frudp_vendor(part->vid));
      break;
    ////////////////////////////////////////////////////////////////////////////
    case FRUDP_PID_PARTICIPANT_GUID:
      guid = (frudp_guid_t *)pval;
      memcpy(&part->guid_prefix, &guid->prefix, FRUDP_GUID_PREFIX_LEN);
      _SPDP_INFO("\tGUID \t\t\t\t\t\t%s\r\n",
                 frudp_print_guid_prefix(&guid->prefix));
      break;
    ////////////////////////////////////////////////////////////////////////////
    case FRUDP_PID_EXPECTS_INLINE_QOS:
      part->expects_inline_qos = *((bool *)pval);
      _SPDP_INFO("\tSPDP vendor_id \t\t\t\t\t0x%04x = %s\r\n",
                 part->vid, frudp_vendor(part->vid));
      break;
    ////////////////////////////////////////////////////////////////////////////
    case FRUDP_PID_METATRAFFIC_UNICAST_LOCATOR:
      loc = (frudp_locator_t *)pval;
      if (frudp_udp_is_same_network(freertps_htonl(loc->addr.udp4.addr), getIp(), getNetwork()))
      {
        part->metatraffic_unicast_locator = *loc; // todo: worry about alignment
        if (loc->kind == FRUDP_LOCATOR_KIND_UDPV4)
        {
          _SPDP_INFO("\tSPDP metatraffic unicast locator udp4: \t\t%s:%d\r\n",
                     frudp_print_ip(freertps_htonl(loc->addr.udp4.addr)),
                     loc->port);
        }
        else if (loc->kind == FRUDP_LOCATOR_KIND_UDPV6)
        { /* ignore ip6 for now...*/ }
        else
          _SPDP_INFO("\tSPDP unknown metatraffic unicast locator kind: %d\r\n",
                     (int)loc->kind);
      }
      else
        _SPDP_INFO("\tSPDP metatraffic unicast locator udp4: \t\t%s:%d (but not on same network)\r\n",
                   frudp_print_ip(freertps_htonl(loc->addr.udp4.addr)),
                   loc->port);
      break;
    ////////////////////////////////////////////////////////////////////////////
    case FRUDP_PID_METATRAFFIC_MULTICAST_LOCATOR:
      loc = (frudp_locator_t *)pval;
      part->metatraffic_multicast_locator = *loc; // todo: worry about alignment
      if (loc->kind == FRUDP_LOCATOR_KIND_UDPV4)
      {
        _SPDP_INFO("\tSPDP metatraffic multicast locator udp4: \t%s:%d\r\n",
                   frudp_print_ip(freertps_htonl(loc->addr.udp4.addr)),
                   loc->port);
      }
      else if (loc->kind == FRUDP_LOCATOR_KIND_UDPV6)
      { /* ignore ip6 for now... */ }
      else
        _SPDP_ERROR("\tSPDP unknown metatraffic multicast locator kind: %d\r\n",
                    (int)loc->kind);
      break;
    ////////////////////////////////////////////////////////////////////////////
    case FRUDP_PID_DEFAULT_UNICAST_LOCATOR:
      loc = (frudp_locator_t *)pval;
      if (frudp_udp_is_same_network(freertps_htonl(loc->addr.udp4.addr), getIp(), getNetwork()))
      {
        part->default_unicast_locator = *loc; // todo: worry about alignment
        if (loc->kind == FRUDP_LOCATOR_KIND_UDPV4)
        {
          _SPDP_INFO("\tSPDP unicast locator udp4: \t\t\t%s:%d\r\n",
                     frudp_print_ip(freertps_htonl(loc->addr.udp4.addr)),
                     loc->port);
        }
      }
      else
      {
        _SPDP_INFO("\tSPDP unicast locator udp4: \t\t\t%s:%d (but not on same network)\r\n",
                   frudp_print_ip(freertps_htonl(loc->addr.udp4.addr)),
                   loc->port);
      }
      break;
    ////////////////////////////////////////////////////////////////////////////
    case FRUDP_PID_DEFAULT_MULTICAST_LOCATOR:
      loc = (frudp_locator_t *)pval;
      part->default_multicast_locator = *loc; // todo: worry about alignment
      if (loc->kind == FRUDP_LOCATOR_KIND_UDPV4)
      {
        _SPDP_INFO("\tSPDP multicast locator udp4: \t\t\t%s:%d\r\n",
                   frudp_print_ip(freertps_htonl(loc->addr.udp4.addr)),
                   loc->port);
      }
      else
      {
        _SPDP_INFO("\tSPDP unknown multicast locator kind: %d\r\n",
                   (int)loc->kind);
      }
      break;
    ////////////////////////////////////////////////////////////////////////////
    case FRUDP_PID_BUILTIN_ENDPOINT_SET:
      part->builtin_endpoints = *((frudp_builtin_endpoint_set_t *)pval);
      _SPDP_INFO("\tbuiltin endpoints: \t\t\t\t0x%08x\r\n",
                 part->builtin_endpoints);
      break;
    ////////////////////////////////////////////////////////////////////////////
    case FRUDP_PID_PARTICIPANT_LEASE_DURATION:
      dur = (frudp_duration_t *)pval;
      part->lease_duration = *dur;
      _SPDP_INFO("\tSPDP lease duration: \t\t\t\t%d.%09d\r\n",
                 dur->sec, dur->nanosec);
      break;
    ////////////////////////////////////////////////////////////////////////////
    case FRUDP_PID_LIVELINESS:
      part->live_count = *(long *)pval;
      _SPDP_INFO("\tSPDP Liveliness: \t\t\t%ld\r\n", part->live_count);
      break;
    ////////////////////////////////////////////////////////////////////////////
    case FRUDP_PID_ENTITY_NAME:
        frudp_parse_string(part->name,
                           sizeof(part->name),
                           (frudp_rtps_string_t *)pval);
      _SPDP_INFO("\tSPDP Name : \t\t\t\t\t%s\r\n", part->name);
      break;
    ////////////////////////////////////////////////////////////////////////////
    default:
      if (!frudp_data_generic(pid)) {
          _SPDP_ERROR("\tunhandled SPDP rx param 0x%x len %d\r\n",
                      (unsigned)pid, item->len);
      }
    }

    // now, advance to next item in list...
    item = (frudp_parameter_list_item_t *)(((uint8_t *)item) + 4 + item->len);
  }

  // now that we have stuff the "part" buffer, spin through our
  // participant list and see if we already have this one
  bool found = false;
  for (int i = 0; !found && i < g_frudp_disco_num_parts; i++)
  {
    frudp_part_t *p = &g_frudp_disco_parts[i];
    if (frudp_guid_prefix_identical(&p->guid_prefix,
                                    &part->guid_prefix))
    {
      _SPDP_INFO("\tfound match at participant slot %d\r\n", i);
      found = true;
      // TODO: see if anything has changed. update if needed
      p->last_spdp = fr_time_now();
    }
  }

  if (!found)
  {
    _SPDP_DEBUG("      didn't have this participant already.\r\n");
    if (g_frudp_disco_num_parts < FRUDP_DISCO_MAX_PARTS)
    {
      _SPDP_INFO("      saved new participant in slot %d\r\n", g_frudp_disco_num_parts);

      frudp_part_t *p = &g_frudp_disco_parts[g_frudp_disco_num_parts];
      part->last_spdp = fr_time_now();
      *p = *part; // save everything plz
      g_frudp_disco_num_parts++;

      frudp_sedp_add_builtin_endpoints(p);
      // push this new participant our SEDP data to speed up the process
    }
    else
      _SPDP_ERROR("      not enough room to save the new participant.\r\n");
  }
//#ifdef VERBOSE_SPDP
  frudp_print_participants_debug();
//#endif
}

void frudp_spdp_init(void)
{
  _SPDP_DEBUG("frudp_spdp_init()\r\n");
  _SEDP_INFO("SPDP => Simple Participant Discovery Protocol\r\n");

  g_frudp_spdp_last_bcast.seconds = 0;
  g_frudp_spdp_last_bcast.fraction = 0;

  frudp_reader_t spdp_reader;
  spdp_reader.writer_guid = g_frudp_guid_unknown;
  spdp_reader.reader_eid = g_spdp_reader_id;
  spdp_reader.max_rx_sn.low = 0;
  spdp_reader.max_rx_sn.high = 0;
  spdp_reader.data_cb = frudp_spdp_rx_data;
  spdp_reader.msg_cb = NULL;
  spdp_reader.reliable = false;
  frudp_add_reader(&spdp_reader);

  _SPDP_INFO("SPDP Reader init\r\n");
  /*
  frudp_subscribe(g_frudp_entity_id_unknown,
                  g_spdp_writer_id,
                  frudp_spdp_rx_data,
                  NULL);
  */
}

void frudp_spdp_start(void)
{
  _SPDP_DEBUG("frudp_spdp_start()\r\n");
  frudp_spdp_tick();
}

void frudp_spdp_fini(void)
{
  _SPDP_DEBUG("SPDP fini\r\n");
}

// todo: this will all eventually be factored somewhere else. for now,
// just work through what it takes to send messages

// todo: consolidate spdp and sedp into a 'discovery' module

/*
uint16_t frudp_append_submsg(frudp_msg_t *msg, const uint16_t msg_wpos,
                             const frudp_submsg_t * const submsg)
{
  frudp_submsg_t *s = (frudp_submsg_t *)&msg->submsgs[msg_wpos];
  memcpy(s, submsg, submsg->header.len);
  return msg_wpos + submsg->header.len;
}
*/

void frudp_spdp_bcast(frudp_part_t *part)
{
  _SPDP_DEBUG("frudp_spdp_bcast()\r\n");

  frudp_msg_t *msg = frudp_init_msg((frudp_msg_t *)g_frudp_disco_tx_buf);
  fr_time_t t = fr_time_now();
  uint16_t submsg_wpos = 0;
  frudp_locator_t *loc = NULL;

  /////////////////////////////////////////////////////////////////////////////
  // TimeSpan Sub-message
  frudp_submsg_t *ts_submsg = (frudp_submsg_t *)&msg->submsgs[submsg_wpos];
  ts_submsg->header.id = FRUDP_SUBMSG_ID_INFO_TS;
  ts_submsg->header.flags = FRUDP_FLAGS_LITTLE_ENDIAN;
  ts_submsg->header.len = 8;
  memcpy(ts_submsg->contents, &t, ts_submsg->header.len);
  submsg_wpos += 4 + ts_submsg->header.len;

  /////////////////////////////////////////////////////////////////////////////
  // Data SPDP Sub-message
  frudp_submsg_data_t *data_submsg = (frudp_submsg_data_t *)&msg->submsgs[submsg_wpos];
  //(frudp_submsg_data_t *)data_submsg->contents;
  data_submsg->header.id = FRUDP_SUBMSG_ID_DATA;
  data_submsg->header.flags = FRUDP_FLAGS_LITTLE_ENDIAN |
//                              FRUDP_FLAGS_INLINE_QOS    | //OLD Disabled (not send in eProsima FastRTPS)
                              FRUDP_FLAGS_DATA_PRESENT  ;
  data_submsg->header.len = 336; //TODO need to compute this dynamically?
  data_submsg->extraflags = 0;
  data_submsg->octets_to_inline_qos = 16; // ?
  data_submsg->reader_id = g_spdp_reader_id; // g_frudp_efrudp_submsg_data_tid_unknown;
  data_submsg->writer_id = g_spdp_writer_id;
  data_submsg->writer_sn.high = 0;
  //static uint32_t bcast_count = 0;
  data_submsg->writer_sn.low = 1; //++bcast_count;

  /////////////////////////////////////////////////////////////////////////////
  // FRUDP_PID_KEY_HASH
  //TODO Disabled (not send in eProsima FastRTPS)
//  frudp_parameter_list_item_t *inline_qos_param =
//    (frudp_parameter_list_item_t *)(((uint8_t *)data_submsg) +
//                                    sizeof(frudp_submsg_data_t));
//  inline_qos_param->pid = FRUDP_PID_KEY_HASH;
//  inline_qos_param->len = 16;
//  memcpy(inline_qos_param->value, &g_frudp_config.guid_prefix, 12);
//  // now i don't know what i'm doing
//  inline_qos_param->value[12] = 0;
//  inline_qos_param->value[13] = 0;
//  inline_qos_param->value[14] = 1;
//  inline_qos_param->value[15] = 0xc1;
//
//  FRUDP_PLIST_ADVANCE(inline_qos_param);
//  /////////////////////////////////////////////////////////////
//  // FRUDP_PID_SENTINEL
//  inline_qos_param->pid = FRUDP_PID_SENTINEL;
//  inline_qos_param->len = 0;

  /////////////////////////////////////////////////////////////////////////////
  // SCHEME ??
  frudp_encapsulation_scheme_t *scheme =
    (frudp_encapsulation_scheme_t *)(((uint8_t *)data_submsg) + sizeof(frudp_submsg_data_t));
  scheme->scheme = freertps_htons(FRUDP_SCHEME_PL_CDR_LE);
  scheme->options = 0;

  /////////////////////////////////////////////////////////////////////////////
  // FRUDP_PID_PROTOCOL_VERSION
  frudp_parameter_list_item_t *param_list =
    (frudp_parameter_list_item_t *)(((uint8_t *)scheme) + sizeof(*scheme));
  param_list->pid = FRUDP_PID_PROTOCOL_VERSION;
  param_list->len = 4;
  param_list->value[0] = 2;
  param_list->value[1] = 1;
  param_list->value[2] = 0;
  param_list->value[3] = 0; // pad to 4-byte boundary

  FRUDP_PLIST_ADVANCE(param_list);
  /////////////////////////////////////////////////////////////
  // FRUDP_PID_VENDOR_ID
  param_list->pid = FRUDP_PID_VENDOR_ID;
  param_list->len = 4;
  param_list->value[0] = (FREERTPS_VID_FREERTPS >> 8) & 0xff;
  param_list->value[1] = FREERTPS_VID_FREERTPS & 0xff;
  param_list->value[2] = 0;
  param_list->value[3] = 0; // pad to 4-byte boundary

  FRUDP_PLIST_ADVANCE(param_list);
  /////////////////////////////////////////////////////////////
  // FRUDP_PID_PARTICIPANT_GUID
  param_list->pid = FRUDP_PID_PARTICIPANT_GUID;
  param_list->len = sizeof(frudp_guid_t); // old 16;
  frudp_guid_t *guid = (frudp_guid_t *)param_list->value;
  memcpy(&guid->prefix, &g_frudp_config.guid_prefix,
           sizeof(frudp_guid_prefix_t));
  guid->eid.s.key[0] = 0;
  guid->eid.s.key[1] = 0;
  guid->eid.s.key[2] = 1;
  guid->eid.s.kind = 0xc1; // => Built-in Participant.

  FRUDP_PLIST_ADVANCE(param_list);
  /////////////////////////////////////////////////////////////
  // FRUDP_PID_METATRAFFIC_MULTICAST_LOCATOR
  param_list->pid = FRUDP_PID_METATRAFFIC_MULTICAST_LOCATOR;
  param_list->len = sizeof(frudp_locator_t);
  loc = (frudp_locator_t *)param_list->value;
  loc->kind = FRUDP_LOCATOR_KIND_UDPV4;
  loc->port = frudp_mcast_builtin_port();
  memset(loc->addr.udp4.zeros, 0, 12);
  loc->addr.udp4.addr = freertps_htonl(FRUDP_DEFAULT_MCAST_GROUP);

  FRUDP_PLIST_ADVANCE(param_list);
  /////////////////////////////////////////////////////////////
  // FRUDP_PID_METATRAFFIC_UNICAST_LOCATOR
  param_list->pid = FRUDP_PID_METATRAFFIC_UNICAST_LOCATOR;
  param_list->len = sizeof(frudp_locator_t);
  loc = (frudp_locator_t *)param_list->value;
  loc->kind = FRUDP_LOCATOR_KIND_UDPV4;
  loc->port = frudp_ucast_builtin_port();
  memset(loc->addr.udp4.zeros, 0, 12);
  loc->addr.udp4.addr = g_frudp_config.unicast_addr;

  FRUDP_PLIST_ADVANCE(param_list);
  /////////////////////////////////////////////////////////////
  // FRUDP_PID_DEFAULT_MULTICAST_LOCATOR
  //TODO Disabled (not send in eProsima FastRTPS)
  param_list->pid = FRUDP_PID_DEFAULT_MULTICAST_LOCATOR;
  param_list->len = sizeof(frudp_locator_t);
  loc = (frudp_locator_t *)param_list->value;
  loc->kind = FRUDP_LOCATOR_KIND_UDPV4;
  loc->port = frudp_mcast_user_port();
  memset(loc->addr.udp4.zeros, 0, 12);
  loc->addr.udp4.addr = freertps_htonl(FRUDP_DEFAULT_MCAST_GROUP);

  FRUDP_PLIST_ADVANCE(param_list);
  /////////////////////////////////////////////////////////////
  // FRUDP_PID_DEFAULT_UNICAST_LOCATOR
  param_list->pid = FRUDP_PID_DEFAULT_UNICAST_LOCATOR;
  param_list->len = sizeof(frudp_locator_t);
  loc = (frudp_locator_t *)param_list->value;
  loc->kind = FRUDP_LOCATOR_KIND_UDPV4;
  loc->port = frudp_ucast_user_port();
  memset(loc->addr.udp4.zeros, 0, 12);
  loc->addr.udp4.addr = g_frudp_config.unicast_addr;

  FRUDP_PLIST_ADVANCE(param_list);
  /////////////////////////////////////////////////////////////
  // FRUDP_PID_PARTICIPANT_LEASE_DURATION
  param_list->pid = FRUDP_PID_PARTICIPANT_LEASE_DURATION;
  param_list->len = sizeof(frudp_duration_t); // old 8;
  frudp_duration_t *duration = (frudp_duration_t *)param_list->value;
  duration->sec = FRUDP_SPDP_LEAVE_DURATION;
  duration->nanosec = 0;

  FRUDP_PLIST_ADVANCE(param_list);
  /////////////////////////////////////////////////////////////
  // FRUDP_PID_BUILTIN_ENDPOINT_SET
  param_list->pid = FRUDP_PID_BUILTIN_ENDPOINT_SET;
  param_list->len = 4;
  uint32_t endpoint_set = 0xc3f; // old 0x3f;
  memcpy(param_list->value, &endpoint_set, 4);

  FRUDP_PLIST_ADVANCE(param_list);
  /////////////////////////////////////////////////////////////
  // FRUDP_PID_ENTITY_NAME
  const char *topic_name = "freertps_node";
  param_list->pid = FRUDP_PID_ENTITY_NAME;
  set_string_alligned(topic_name, param_list);

  FRUDP_PLIST_ADVANCE(param_list);
  /////////////////////////////////////////////////////////////
  // FRUDP_PID_SENTINEL
  param_list->pid = FRUDP_PID_SENTINEL;
  param_list->len = 0;

  FRUDP_PLIST_ADVANCE(param_list);
  /////////////////////////////////////////////////////////////
  //data_submsg->header.len = next_submsg_ptr - data_submsg->contents;
  data_submsg->header.len = param_list->value - 4 - (uint8_t *)&data_submsg->extraflags;
  frudp_submsg_t *next_submsg_ptr = (frudp_submsg_t *)param_list;

  /////////////////////////////////////////////////////////////
//  ts_submsg = (frudp_submsg_t *)param_list;
//  ts_submsg->header.id = FRUDP_SUBMSG_ID_INFO_TS;
//  ts_submsg->header.flags = FRUDP_FLAGS_LITTLE_ENDIAN;
//  ts_submsg->header.len = 8;
//  memcpy(ts_submsg->contents, &t, 8);
//  uint8_t *next_submsg_ptr = ((uint8_t *)param_list) + 4 + 8;

  /////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////
  //data_submsg->header.len = next_submsg_ptr - data_submsg->contents;
  //_SPDP_INFO("len = %d\r\n", data_submsg->header.len);
  /////////////////////////////////////////////////////////////
  //int payload_len = ((uint8_t *)param_list) - ((uint8_t *)msg->submsgs);
  //int payload_len = ((uint8_t *)next_submsg_ptr) - ((uint8_t *)msg->submsgs);
  int payload_len = ((uint8_t *)next_submsg_ptr) - ((uint8_t *)msg);

  uint32_t addr = 0;
  uint16_t port = 0;

  // Aggreagte to Global/Multicast a unicast SPDP message
  if (part != NULL) {
    addr = freertps_htonl(part->metatraffic_unicast_locator.addr.udp4.addr);
    port = part->metatraffic_unicast_locator.port;

    if (!frudp_tx(addr, port, (const uint8_t *)msg, payload_len))
      _SPDP_ERROR("couldn't transmit SPDP broadcast message\r\n");
  }

  // Global/Multicast SPDP message
  addr = FRUDP_DEFAULT_MCAST_GROUP;
  port = frudp_mcast_builtin_port();
  if (!frudp_tx(addr, port, (const uint8_t *)msg, payload_len))
    _SPDP_ERROR("couldn't transmit SPDP unicast message\r\n");
}

//#ifdef VERBOSE_SPDP
void frudp_print_participants_debug(void)
{
  fr_time_t time = fr_time_now();
  FREERTPS_INFO("\r\n");
  FREERTPS_INFO("Current time : %d \r\n", fr_time_now().seconds);
  FREERTPS_INFO("PARTICIPANT\r\n");
  FREERTPS_INFO("| ID     | NAME       | IP              | GUID                       | Bail | Last Time  | End |\r\n");
  for (unsigned i = 0; i < g_frudp_disco_num_parts; i++) {
    frudp_part_t *match = &g_frudp_disco_parts[i];
    frudp_duration_t *duration = &match->lease_duration;
    int32_t bail = (duration->sec); // + duration->nanosec); // In second
    FREERTPS_INFO("| %d\t| %s | %s | %s | %d | %d | %d |\r\n",
                  i,
                  match->name,
                  frudp_print_ip(freertps_htonl(match->default_unicast_locator.addr.udp4.addr)),
                  frudp_print_guid_prefix(&match->guid_prefix),
                  bail,
                  match->last_spdp.seconds,
                  (time.seconds - (match->last_spdp.seconds + bail)));
  }
  FREERTPS_INFO("\r\n");
}
//#endif

void frudp_spdp_tick(void)
{
  const fr_time_t t = fr_time_now();
  int32_t delta = fr_time_diff(&t, &g_frudp_spdp_last_bcast).seconds;
//  FREERTPS_INFO(" %d > %d %d\r\n", delta, FRUDP_SPDP_DELAY_SEC, t.seconds);
  if (delta >= FRUDP_SPDP_DELAY_SEC)
  {
    _SPDP_DEBUG("frudp_spdp_tick()\r\n");
    g_frudp_spdp_last_bcast = t;

    frudp_spdp_bcast(NULL);
    frudp_spdp_clean();

//#ifdef VERBOSE_SPDP
    FREERTPS_INFO("\r\n");
    frudp_print_participants_debug();
//#endif
  }
}
