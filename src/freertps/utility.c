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

#include "freertps/utility.h"
#include "freertps/psm/bswap.h"
#include "freertps/rtps/constant/submsg_id.h"

#include <stdio.h>

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

/** Display IP
 * @param ip in normal mode.
 */
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
