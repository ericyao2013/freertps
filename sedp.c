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

#include <string.h>

#include "freertps/freertps.h"
#include "freertps/sedp.h"
#include "freertps/spdp.h"
#include "freertps/qos.h"


// todo: an option to generate SEDP messages on-the-fly as requested,
// rather than buffering them in precious SRAM. or maybe auto-generate the
// pub/sub messages and hold them in flash. That would be way better.
#define SEDP_MSG_BUF_LEN 1024

////////////////////////////////////////////////////////////////////////////
// local constants
static const frudp_eid_t g_sedp_pub_writer_id = { .u = 0xc2030000 };
static const frudp_eid_t g_sedp_pub_reader_id = { .u = 0xc7030000 };
static const frudp_eid_t g_sedp_sub_writer_id = { .u = 0xc2040000 };
static const frudp_eid_t g_sedp_sub_reader_id = { .u = 0xc7040000 };

static sedp_topic_info_t g_topic_info;

////////////////////////////////////////////////////////////////////////////
// local functions
static void frudp_sedp_rx_pub_data(frudp_receiver_state_t *rcvr,
                                   const frudp_submsg_t *submsg,
                                   const uint16_t scheme,
                                   const uint8_t *data);

static void frudp_sedp_rx_sub_data(frudp_receiver_state_t *rcvr,
                                   const frudp_submsg_t *submsg,
                                   const uint16_t scheme,
                                   const uint8_t *data);

static void frudp_sedp_rx_pubsub_data(frudp_receiver_state_t *rcvr,
                                      const frudp_submsg_t *submsg,
                                      const uint16_t scheme,
                                      const uint8_t *data,
                                      const bool is_pub);

static void frudp_sedp_bcast(void);

////////////////////////////////////////////////////////////////////////////
// static globals
static fr_time_t frudp_sedp_last_bcast;
frudp_pub_t *g_sedp_sub_pub = NULL; // SEDP subscription publication
frudp_pub_t *g_sedp_pub_pub = NULL; // SEDP publication publication

//static uint8_t frudp_pub_sample_t[SEDP_MSG_BUF_LEN];
static uint8_t g_sedp_sub_writer_data_buf[SEDP_MSG_BUF_LEN * FRUDP_MAX_SUBS];
static uint8_t g_sedp_pub_writer_data_buf[SEDP_MSG_BUF_LEN * FRUDP_MAX_PUBS];
static frudp_submsg_data_t *g_sedp_sub_writer_data_submsgs[FRUDP_MAX_SUBS];
static frudp_submsg_data_t *g_sedp_pub_writer_data_submsgs[FRUDP_MAX_PUBS];
//sizeof [MAX_SUBS][SEDP_MSG_BUF_LEN];
static uint8_t g_sedp_msg_buf[SEDP_MSG_BUF_LEN];

void frudp_sedp_init(void)
{
  _SEDP_DEBUG("frudp_sedp_init()\r\n");
  _SEDP_INFO("SEDP => Simple Endpoint Discovery Protocol\r\n");

  for (int i = 0; i < FRUDP_MAX_SUBS; i++)
  {
    frudp_submsg_data_t *d = 
      (frudp_submsg_data_t *)&g_sedp_sub_writer_data_buf[i * SEDP_MSG_BUF_LEN];
    d->writer_sn = g_frudp_sn_unknown;
    g_sedp_sub_writer_data_submsgs[i] = d;
  }

  for (int i = 0; i < FRUDP_MAX_PUBS; i++)
  {
    frudp_submsg_data_t *d = 
      (frudp_submsg_data_t *)&g_sedp_pub_writer_data_buf[i * SEDP_MSG_BUF_LEN];
    d->writer_sn = g_frudp_sn_unknown;
    g_sedp_pub_writer_data_submsgs[i] = d;
  }

  g_sedp_sub_pub = frudp_create_pub(
    NULL, // no topic name
    NULL, // no type name
    g_sedp_sub_writer_id,
    g_sedp_sub_writer_data_submsgs,
    FRUDP_MAX_SUBS);

  g_sedp_pub_pub = frudp_create_pub(
    NULL, // no topic name
    NULL, // no type name
    g_sedp_pub_writer_id,
    g_sedp_pub_writer_data_submsgs,
    FRUDP_MAX_PUBS);

  frudp_sub_t sedp_sub_sub;
  sedp_sub_sub.topic_name = NULL;
  sedp_sub_sub.type_name = NULL;
  sedp_sub_sub.reader_eid = g_sedp_sub_reader_id;
  sedp_sub_sub.data_cb = frudp_sedp_rx_sub_data;
  sedp_sub_sub.msg_cb = NULL;
  sedp_sub_sub.reliable = true;
  frudp_add_sub(&sedp_sub_sub);

  frudp_sub_t sedp_pub_sub; // subscribe to the publisher announcers
  sedp_pub_sub.topic_name = NULL;
  sedp_pub_sub.type_name = NULL;
  sedp_pub_sub.reader_eid = g_sedp_pub_reader_id;
  sedp_pub_sub.data_cb = frudp_sedp_rx_pub_data;
  sedp_pub_sub.msg_cb = NULL;
  sedp_pub_sub.reliable = true;
  frudp_add_sub(&sedp_pub_sub);
}

void frudp_sedp_start(void)
{
  _SEDP_DEBUG("frudp_sedp_start()\r\n");
  // go through and send SEDP messages for all of our subscriptions
  for (int i = 0; i < g_frudp_num_subs; i++)
  {
    if (g_frudp_subs[i].topic_name) // user subs have topic names
    {
      _SEDP_DEBUG("sending SEDP message about subscription [%s]\r\n",
                  g_frudp_subs[i].topic_name);
      frudp_sedp_publish_sub(&g_frudp_subs[i]);
    }
  }

  // now go through and send SEDP messages for all our publications
  for (int i = 0; i < g_frudp_num_pubs; i++)
  {
    if (g_frudp_pubs[i].topic_name)
    {
      _SEDP_DEBUG("sending SEDP message about publication [%s]\r\n",
                  g_frudp_pubs[i].topic_name);
      frudp_sedp_publish_pub(&g_frudp_pubs[i]);
    }
  }
}

void frudp_sedp_fini(void)
{
  _SEDP_DEBUG("sedp fini\r\n");
}

void frudp_sedp_tick(void)
{
  _SEDP_DEBUG("frudp_sedp_tick()\r\n");
  const fr_time_t t = fr_time_now();
  if (fr_time_diff(&t, &frudp_sedp_last_bcast).seconds >= 1)
  {
    frudp_sedp_bcast();
    frudp_sedp_last_bcast = t;
  }
}

//#define SEDP_PRINT_TOPICS
static void frudp_sedp_rx_pub_data(frudp_receiver_state_t *rcvr,
                                   const frudp_submsg_t *submsg,
                                   const uint16_t scheme,
                                   const uint8_t *data)
{
  frudp_sedp_rx_pubsub_data(rcvr, submsg, scheme, data, true);
}

static void frudp_sedp_rx_sub_data(frudp_receiver_state_t *rcvr,
                                   const frudp_submsg_t *submsg,
                                   const uint16_t scheme,
                                   const uint8_t *data)
{
  frudp_sedp_rx_pubsub_data(rcvr, submsg, scheme, data, false);
}

static void frudp_sedp_rx_pub_info(const sedp_topic_info_t *info)
{
  _SEDP_DEBUG("frudp_sedp_rx_pub_info()\r\n");
  _SEDP_INFO("    SEDP pub: [%s / %s] num_subs = %d\r\n",
      info->topic_name ? info->topic_name : "",
      info->type_name ? info->type_name : "",
      (int)g_frudp_num_subs);

  // look to see if we are subscribed to this topic
  for (unsigned i = 0; i < g_frudp_num_subs; i++)
  {
    frudp_sub_t *sub = &g_frudp_subs[i];
    _SEDP_DEBUG("\tcomparing incoming SEDP pub to our subscriber %s of type %s\r\n",
           (sub->topic_name ? sub->topic_name : "(no name)"),
           (sub->type_name  ? sub->type_name  : "(no type)"));

    if (!sub->topic_name || !sub->type_name)
      continue; // sanity check. some built-ins don't have names.

    if (!strcmp(sub->topic_name, info->topic_name) &&
        !strcmp(sub->type_name, info->type_name))
    {
      _SEDP_INFO("\thooray! found a topic we care about: [%s]\r\n",
                 sub->topic_name);

      // see if we already have a matched reader for this writer
      bool found = false;
      for (unsigned j = 0; !found && j < g_frudp_num_readers; j++)
      {
        frudp_reader_t *match = &g_frudp_readers[j];
        if (frudp_guid_identical(&match->writer_guid, &info->guid))
          found = true;
      }

      if (!found)
      {
        frudp_reader_t r;
        r.writer_guid = info->guid;
        r.reader_eid = sub->reader_eid;
        r.max_rx_sn.high = 0;
        r.max_rx_sn.low = 0;
        r.data_cb = sub->data_cb;
        r.msg_cb = sub->msg_cb;
        r.reliable = sub->reliable;
        frudp_add_reader(&r);
      }
      else
        _SEDP_INFO("\tboring, we already knew about it.\r\n");
    }
  }
}

static void frudp_sedp_rx_sub_info(const sedp_topic_info_t *info)
{
  _SEDP_DEBUG("frudp_sedp_rx_sub_info()\r\n");
  _SEDP_INFO("\tSEDP sub: [%s]\r\n", info->topic_name ? info->topic_name : "");

  // look to see if we publish this topic
  for (unsigned i = 0; i < g_frudp_num_pubs; i++)
  {
    frudp_pub_t *pub = &g_frudp_pubs[i];
    _SEDP_INFO("\tcomparing incoming SEDP sub to our publisher %s of type %s\r\n",
           (pub->topic_name ? pub->topic_name : "(no name)"),
           (pub->type_name  ? pub->type_name  : "(no type)"));
           
    if (!pub->topic_name || !pub->type_name) {
      _SEDP_INFO("\tSEDP empty topic or type\r\n");
      continue; // sanity check. some built-ins don't have names.
    }

    if (strcmp(pub->topic_name, info->topic_name)) {
      _SEDP_INFO("\tSEDP not the same topic\r\n");
      continue; // not the same topic. move along.
    }

    if (strcmp(pub->type_name, info->type_name)) {
      _SEDP_INFO("\tSEDP type mismatch: [%s] != [%s]\r\n",
                 pub->type_name, info->type_name);
      continue;
    }

    _SEDP_INFO("\thooray! heard a request for a topic we publish: [%s]\r\n",
               pub->topic_name);

    // see if we already have a writer for this subscriber
    bool found = false;
    for (unsigned j = 0; !found && j < g_frudp_num_writers; j++) {
      frudp_writer_t *w = &g_frudp_writers[j];
      if (frudp_guid_identical(&w->reader_guid, &info->guid)) {
        _SEDP_INFO("\tnah, already had it in our list of readers\r\n");
        found = true;
      }
    }

    if (!found)
    {
      frudp_writer_t w;
      w.reader_guid = info->guid;
      w.writer_eid = pub->writer_eid;
      frudp_add_writer(&w);
    }
    else
      _SEDP_INFO("\tboring, we already knew about it.\r\n");
  }
}

static void frudp_sedp_rx_pubsub_data(frudp_receiver_state_t *rcvr,
                                      const frudp_submsg_t *submsg,
                                      const uint16_t scheme,
                                      const uint8_t *data,
                                      const bool is_pub)
{
  _SEDP_DEBUG("frudp_sedp_rx_pubsub_data()\r\n");
  _SEDP_INFO("    SEDP rx:\r\n");

  if (scheme != FRUDP_SCHEME_PL_CDR_LE) {
    _SEDP_ERROR("expected SEDP data to be PL_CDR_LE. bailing...\r\n");
    return;
  }

  memset(&g_topic_info, 0, sizeof(sedp_topic_info_t));
  frudp_parameter_list_item_t *item = (frudp_parameter_list_item_t *)data;
  while ((uint8_t *)item < submsg->contents + submsg->header.len)
  {
    const frudp_parameterid_t pid = item->pid;
    if (pid == FRUDP_PID_SENTINEL)
      break;

    const uint8_t *pval = item->value;
        _SPDP_DEBUG("\tunhandled SEDP rx param 0x%x len %d\r\n",
                      (unsigned)pid, item->len);

    if (pid == FRUDP_PID_ENDPOINT_GUID) {
      frudp_guid_t *guid = (frudp_guid_t *)pval;
      memcpy(&g_topic_info.guid, guid, sizeof(frudp_guid_t));
      _SEDP_INFO("\tSEDP endpoint guid : \t%s\r\n",
                 frudp_print_guid_prefix(&guid->prefix));

      //if (guid->entity_id.u == 0x03010000)
      //  _info("found entity 0x103\n");
    }

    else if (pid == FRUDP_PID_TOPIC_NAME)
    {
      if (frudp_parse_string(g_topic_info.topic_name,
                             sizeof(g_topic_info.topic_name),
                             (frudp_rtps_string_t *)pval)) {
        _SEDP_INFO("\tSEDP topic name: \t\t[%s]\r\n", g_topic_info.topic_name);
      } else _SEDP_ERROR("\tcouldn't parse topic name\r\n");
    }

    else if (pid == FRUDP_PID_TYPE_NAME)
    {
      if (frudp_parse_string(g_topic_info.type_name,
                             sizeof(g_topic_info.type_name),
                             (frudp_rtps_string_t *)pval)) {
        _SEDP_INFO("\tSEDP type name: \t\t[%s]\r\n", g_topic_info.type_name);
      } else _SEDP_ERROR("\tcouldn't parse type name\r\n");
    }

    else if (pid == FRUDP_PID_RELIABILITY)
    {
      frudp_qos_reliability_t *qos = (frudp_qos_reliability_t *)pval;
      if (qos->kind == FRUDP_QOS_RELIABILITY_KIND_BEST_EFFORT) {
        _SEDP_INFO("\tSEDP reliability QoS: \t[best-effort]\r\n");
      }
      else if (qos->kind == FRUDP_QOS_RELIABILITY_KIND_RELIABLE) {
        _SEDP_INFO("\tSEDP reliability QoS: \t[reliable]\r\n");
      }
      else _SEDP_ERROR("\tunhandled reliability kind: %d\r\n", (int)qos->kind);
    }
    else if (pid == FRUDP_PID_HISTORY)
    {
      frudp_qos_history_t *qos = (frudp_qos_history_t *)pval;
      if (qos->kind == FRUDP_QOS_HISTORY_KIND_KEEP_LAST)
      {
        _SEDP_INFO("\thistory QoS: \t[keep last %d]\r\n", (int)qos->depth);
      }
      else if (qos->kind == FRUDP_QOS_HISTORY_KIND_KEEP_ALL)
      {
        _SEDP_INFO("\thistory QoS: \t[keep all]\r\n");
      }
      else _SEDP_ERROR("\tunhandled history kind: %d\r\n", (int)qos->kind);
    }
    else if (pid == FRUDP_PID_TRANSPORT_PRIORITY)
    {
      _SEDP_INFO("\ttransport priority: \t%d\r\n", (int)*((uint32_t *)pval));
    }
    // now, advance to next item in list...
    item = (frudp_parameter_list_item_t *)(((uint8_t *)item) + 4 + item->len);
  }

  // make sure we have received all necessary parameters
  if (!strlen(g_topic_info.type_name) ||
      !strlen(g_topic_info.topic_name) ||
      frudp_guid_identical(&g_topic_info.guid, &g_frudp_guid_unknown)) {
    _SEDP_ERROR("\tinsufficient SEDP information\r\n");
    return;
  }

  if (is_pub) // this is information about someone else's publication
    frudp_sedp_rx_pub_info(&g_topic_info);
  else // this is information about someone else's subscription
    frudp_sedp_rx_sub_info(&g_topic_info);
}

static void frudp_sedp_bcast(void)
{
  _SEDP_DEBUG("   frudp_sedp_bcast()\r\n");
  //frudp_msg_t *msg = frudp_init_msg((frudp_msg_t *)g_frudp_discovery_tx_buf);
  //fr_time_t t = fr_time_now();
}

static void frudp_sedp_publish(const char *topic_name,
                         const char *type_name,
                         frudp_pub_t *pub,
                         const frudp_eid_t eid,
                         const bool is_pub) // is this for a pub or a sub
{
  _SEDP_DEBUG("frudp_sedp_publish()\r\n");
  // first make sure we have an spdp packet out first
  _SEDP_INFO("\tSEDP publish [%s] via SEDP EID 0x%08x\r\n",
      topic_name, (unsigned)freertps_htonl(pub->writer_eid.u));

  frudp_submsg_data_t *d = (frudp_submsg_data_t *)g_sedp_msg_buf;
  d->header.id = FRUDP_SUBMSG_ID_DATA;
  d->header.flags = FRUDP_FLAGS_LITTLE_ENDIAN |
                    //FRUDP_FLAGS_INLINE_QOS    |
                    FRUDP_FLAGS_DATA_PRESENT  ;
  d->header.len = 0;
  d->extraflags = 0;
  d->octets_to_inline_qos = 16; // ?
  d->reader_id = is_pub ? g_sedp_pub_reader_id : g_sedp_sub_reader_id;
  d->writer_id = is_pub ? g_sedp_pub_writer_id : g_sedp_sub_writer_id;
  //d->writer_sn = g_frudp_sn_unknown;
  d->writer_sn.high = 0;
  d->writer_sn.low = 0; // todo: increment this
  //frudp_parameter_list_item_t *inline_qos_param =
  //  (frudp_parameter_list_item_t *)d->data;
  /*
  inline_qos_param->pid = FRUDP_PID_KEY_HASH;
  inline_qos_param->len = 16;
  memcpy(inline_qos_param->value, &reader_guid, 16);
  */

  /////////////////////////////////////////////////////////////
  frudp_encapsulation_scheme_t *scheme =
    (frudp_encapsulation_scheme_t *)((uint8_t *)d->data);
  scheme->scheme = freertps_htons(FRUDP_SCHEME_PL_CDR_LE);
  scheme->options = 0;

  /////////////////////////////////////////////////////////////
  frudp_parameter_list_item_t *param =
    (frudp_parameter_list_item_t *)(((uint8_t *)scheme) + sizeof(*scheme));

  /////////////////////////////////////////////////////////////
  param->pid = FRUDP_PID_PROTOCOL_VERSION;
  param->len = 4;
  param->value[0] = 2;
  param->value[1] = 1;
  param->value[2] = param->value[3] = 0; // pad to 4-byte boundary

  /////////////////////////////////////////////////////////////
  FRUDP_PLIST_ADVANCE(param);
  param->pid = FRUDP_PID_VENDOR_ID;
  param->len = 4;
  param->value[0] = (FREERTPS_VENDOR_ID >> 8) & 0xff;
  param->value[1] = FREERTPS_VENDOR_ID & 0xff;
  param->value[2] = param->value[3] = 0; // pad to 4-byte boundary

  /////////////////////////////////////////////////////////////
  FRUDP_PLIST_ADVANCE(param);
  param->pid = FRUDP_PID_ENDPOINT_GUID;
  param->len = 16;
  frudp_guid_t guid;
  guid.prefix = g_frudp_config.guid_prefix;
  guid.eid = eid;
  memcpy(param->value, &guid, 16);

//#ifdef VERBOSE_SEDP
//  FREERTPS_INFO("reader_guid = 0x%08x\n", htonl(reader_guid.entity_id.u));
//#endif

  /////////////////////////////////////////////////////////////
  if (topic_name)
  {
    FRUDP_PLIST_ADVANCE(param);
    param->pid = FRUDP_PID_TOPIC_NAME;
    int topic_len = topic_name ? strlen(topic_name) : 0;
    uint32_t *param_topic_len = (uint32_t *)param->value;
    *param_topic_len = topic_len + 1;
    //*((uint32_t *)param->value) = topic_len + 1;
    memcpy(param->value + 4, topic_name, topic_len + 1);
    //param->value[4 + topic_len + 1] = 0; // null-terminate plz
    param->len = (4 + topic_len + 1 + 3) & ~0x3; // params must be 32-bit aligned
  }

  /////////////////////////////////////////////////////////////
  if (type_name)
  {
    FRUDP_PLIST_ADVANCE(param);
    param->pid = FRUDP_PID_TYPE_NAME;
    int type_len = strlen(type_name);
    uint32_t *value = (uint32_t *)param->value;
    *value = type_len + 1;
    memcpy(param->value + 4, type_name, type_len + 1);
    param->len = (4 + type_len + 1 + 3) & ~0x3; // params must be 32-bit aligned
  }

  /////////////////////////////////////////////////////////////
  // todo: follow the "reliable" flag in the subscription structure
  FRUDP_PLIST_ADVANCE(param);
  param->pid = FRUDP_PID_RELIABILITY;
  param->len = 12;
  frudp_qos_reliability_t *reliability = (frudp_qos_reliability_t *)param->value;
  reliability->kind = FRUDP_QOS_RELIABILITY_KIND_BEST_EFFORT;
  reliability->max_blocking_time.sec = 0;
  reliability->max_blocking_time.nanosec = 0x19999999; // todo: figure this out

  /////////////////////////////////////////////////////////////
  /*
  FRUDP_PLIST_ADVANCE(param);
  param->pid = FRUDP_PID_PRESENTATION;
  param->len = 8;
  frudp_qos_presentation_t *presentation = (frudp_qos_presentation_t *)param->value;
  presentation->scope = FRUDP_QOS_PRESENTATION_SCOPE_TOPIC;
  presentation->coherent_access = 0;
  presentation->ordered_access = 0;
  */

  /////////////////////////////////////////////////////////////
  FRUDP_PLIST_ADVANCE(param);
  param->pid = FRUDP_PID_SENTINEL;
  param->len = 0;
  FRUDP_PLIST_ADVANCE(param);
  d->header.len = param->value - 4 - (uint8_t *)&d->extraflags;
  frudp_publish(pub, d); // this will be either on the sub or pub publisher
}

void frudp_sedp_publish_sub(frudp_sub_t *sub)
{
  if (!g_sedp_sub_pub)
  {
    _SEDP_INFO("woah there partner.\r\n"
           "you need to call frudp_part_create()\r\n");
    return;
  }

  _SEDP_DEBUG("sedp_publish_sub(%s)\r\n", sub->topic_name);
  frudp_sedp_publish(sub->topic_name,
               sub->type_name,
               g_sedp_sub_pub,
               sub->reader_eid,
               false); // false means "this is for a subscription"
}

void frudp_sedp_publish_pub(frudp_pub_t *pub)
{
  if (!g_sedp_pub_pub)
  {
    _SEDP_INFO("woah there partner.\r\n"
           "you need to call frudp_part_create()\r\n");
    return;
  }

  _SEDP_DEBUG("sedp_publish_pub(%s)\r\n", pub->topic_name);
  frudp_sedp_publish(pub->topic_name,
               pub->type_name,
               g_sedp_pub_pub,
               pub->writer_eid,
               true); // true means "this is for a publication"
}

void frudp_sedp_add_builtin_endpoints(frudp_part_t *part)
{
  _SEDP_DEBUG("frudp_sedp_add_builtin_endpoints()\r\n");
  _SEDP_INFO("    adding endpoints for %s\r\n",
             frudp_print_guid_prefix(&part->guid_prefix));

  // this reads the remote peer's publications
  frudp_reader_t pub_reader;
  pub_reader.writer_guid = g_frudp_guid_unknown;
  frudp_stuff_guid(&pub_reader.writer_guid,
                   &part->guid_prefix,
                   &g_sedp_pub_writer_id);
  pub_reader.reader_eid = g_sedp_pub_reader_id;
  pub_reader.max_rx_sn.low = 0;
  pub_reader.max_rx_sn.high = 0;
  pub_reader.data_cb = frudp_sedp_rx_pub_data;
  pub_reader.msg_cb = NULL;
  pub_reader.reliable = true;
  frudp_add_reader(&pub_reader);

  // this reads the remote peer's subscriptions
  frudp_reader_t sub_reader;
  sub_reader.writer_guid = g_frudp_guid_unknown;
  frudp_stuff_guid(&sub_reader.writer_guid,
                   &part->guid_prefix,
                   &g_sedp_sub_writer_id);
  sub_reader.reader_eid = g_sedp_sub_reader_id;
  sub_reader.max_rx_sn.low = 0;
  sub_reader.max_rx_sn.high = 0;
  sub_reader.data_cb = frudp_sedp_rx_sub_data;
  sub_reader.msg_cb = NULL;
  sub_reader.reliable = true;
  frudp_add_reader(&sub_reader);

  // blast our SEDP data at this participant
  frudp_send_sedp_msgs(part);
}


#if 0
/////////////////////////////////////////////////////////////
PLIST_ADVANCE(param_list);
param_list->pid = FRUDP_PID_BUILTIN_ENDPOINT_SET;
param_list->len = 4;
uint32_t endpoint_set = 0x3f;
memcpy(param_list->value, &endpoint_set, 4);
/////////////////////////////////////////////////////////////
PLIST_ADVANCE(param_list);
param_list->pid = FRUDP_PID_SENTINEL;
param_list->len = 0;
//data_submsg->header.len = next_submsg_ptr - data_submsg->contents;
PLIST_ADVANCE(param_list);
data_submsg->header.len = param_list->value - 4 - data_submsg->contents;
frudp_submsg_t *next_submsg_ptr = (frudp_submsg_t *)param_list;
/////////////////////////////////////////////////////////////
/*
ts_submsg = (frudp_submsg_t *)param_list;
ts_submsg->header.id = FRUDP_SUBMSG_ID_INFO_TS;
ts_submsg->header.flags = FRUDP_FLAGS_LITTLE_ENDIAN;
ts_submsg->header.len = 8;
memcpy(ts_submsg->contents, &t, 8);
uint8_t *next_submsg_ptr = ((uint8_t *)param_list) + 4 + 8;
*/

#endif
