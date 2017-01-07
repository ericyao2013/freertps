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

#include <stdio.h>
#include <string.h>

#include "freertps/freertps.h"
#include "freertps/bswap.h"
#include "freertps/id.h"

static unsigned g_frudp_next_user_eid = 1;

const frudp_guid_t g_frudp_guid_unknown =
    { .prefix = { .prefix = {0} }, .eid = { .u = 0 } };

const char *frudp_submsg(const uint16_t id)
{
  switch (id)
  {
    case FRUDP_SUBMSG_ID_PAD:               return "Pad";
    case FRUDP_SUBMSG_ID_ACKNACK:           return "AckNack";
    case FRUDP_SUBMSG_ID_HEARTBEAT:         return "Heartbeat";
    case FRUDP_SUBMSG_ID_GAP:               return "Gap";
    case FRUDP_SUBMSG_ID_INFO_TS:           return "InfoTimestamp";
    case FRUDP_SUBMSG_ID_INFO_SRC:          return "InfoSource";
    case FRUDP_SUBMSG_ID_INFO_REPLY_IP4:    return "InfoReplyIp4";
    case FRUDP_SUBMSG_ID_INFO_DEST:         return "InfoDestination";
    case FRUDP_SUBMSG_ID_INFO_REPLY:        return "InfoReply";
    case FRUDP_SUBMSG_ID_NACK_FRAG:         return "NackFrag";
    case FRUPG_SUBMSG_ID_HEARTBEAT_FRAG:    return "HeartbeatFrag";
    case FRUDP_SUBMSG_ID_DATA:              return "Data";
    case FRUDP_SUBMSG_ID_DATA_FRAG:         return "DataFrag";
    default:                                return "unknown";
  }
}

const char *frudp_vendor(const frudp_vid_t vid)
{
  switch (vid)
  {
    case 0x0101: return "RTI Connext";
    case 0x0102: return "PrismTech OpenSplice";
    case 0x0103: return "OCI OpenDDS";
    case 0x0104: return "MilSoft";
    case 0x0105: return "Gallium InterCOM";
    case 0x0106: return "TwinOaks CoreDX";
    case 0x0107: return "Lakota Technical Systems";
    case 0x0108: return "ICOUP Consulting";
    case 0x0109: return "ETRI";
    case 0x010a: return "RTI Connext Micro";
    case 0x010b: return "PrismTech Vortex Cafe";
    case 0x010c: return "PrismTech Vortex Gateway";
    case 0x010d: return "PrismTech Vortex Lite";
    case 0x010e: return "Technicolor Qeo";
    case 0x010f: return "eProsima";
    case 0x0120: return "PrismTech Vortex Cloud";
    case FREERTPS_VENDOR_ID: return "freertps";
    case 0x2f2c: return "freertps";
    default:     return "unknown";
  }
}

bool frudp_guid_prefix_identical(frudp_guid_prefix_t * const a,
                                 frudp_guid_prefix_t * const b)
{
//  FREERTPS_DEBUG("frudp_guid_prefix_identical(a,b)\r\n");
  for (int i = 0; i < FRUDP_GUID_PREFIX_LEN; i++)
    if (a->prefix[i] != b->prefix[i])
      return false;
  return true;
}

bool frudp_guid_identical(const frudp_guid_t * const a,
                          const frudp_guid_t * const b)
{
//  FREERTPS_DEBUG("frudp_guid_identical(a,b)\r\n");
  if (a->eid.u != b->eid.u)
    return false;

  for (int i = 0; i < FRUDP_GUID_PREFIX_LEN; i++)
    if (a->prefix.prefix[i] != b->prefix.prefix[i])
      return false;
  return true;
}

const char *frudp_print_guid_prefix(const frudp_guid_prefix_t *p)
{
  static char buffer[26];
  snprintf(buffer, sizeof(buffer),
           "%02x%02x%02x%02x:%02x%02x%02x%02x:%02x%02x%02x%02x",
           (unsigned)p->prefix[0],
           (unsigned)p->prefix[1],
           (unsigned)p->prefix[2],
           (unsigned)p->prefix[3],
           (unsigned)p->prefix[4],
           (unsigned)p->prefix[5],
           (unsigned)p->prefix[6],
           (unsigned)p->prefix[7],
           (unsigned)p->prefix[8],
           (unsigned)p->prefix[9],
           (unsigned)p->prefix[10],
           (unsigned)p->prefix[11]);
  return buffer;
}

const char *frudp_print_ip(const long ip)
{
  static char buffer[16];
  snprintf(buffer, sizeof(buffer),
           "%03d.%03d.%03d.%03d",
           (unsigned int)FREERTPS_IPV4_BYTE(ip,3),
           (unsigned int)FREERTPS_IPV4_BYTE(ip,2),
           (unsigned int)FREERTPS_IPV4_BYTE(ip,1),
           (unsigned int)FREERTPS_IPV4_BYTE(ip,0));
  return buffer;
}

void frudp_stuff_guid(frudp_guid_t *guid,
                      const frudp_guid_prefix_t *prefix,
                      const frudp_eid_t *id)
{
//  FREERTPS_DEBUG("frudp_stuff_guid(guid,prefix,id) make copy...\r\n");
  memcpy(&guid->prefix, prefix, sizeof(frudp_guid_prefix_t));
  guid->eid = *id;
}

const char *frudp_print_guid(const frudp_guid_t *guid)
{
  static char buffer[36];
  snprintf(buffer, sizeof(buffer),
           "%s:%08x",
           frudp_print_guid_prefix(&guid->prefix),
           (unsigned)freertps_htonl(guid->eid.u));
  return buffer;
}

frudp_eid_t frudp_create_user_id(const uint8_t entity_kind)
{
  FREERTPS_DEBUG("frudp_create_user_id()\r\n");

  frudp_eid_t eid;
  eid.s.kind = entity_kind; // entity kind must be set by caller of this function must be overwritten by FRUDP_ENTITY_KIND_USER_READER_NO_KEY; // has key? dunno
  eid.s.key[0] = 0;
  eid.s.key[1] = 0; // todo: >8 bit ID's
  eid.s.key[2] = g_frudp_next_user_eid++;
  return eid;
}
