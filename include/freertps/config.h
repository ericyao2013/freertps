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

#ifndef FREERTPS_CONFIG_H
#define FREERTPS_CONFIG_H

#include <stdint.h>
#include "freertps/id.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define FRUDP_DOMAIN_ID  0

#define FRUDP_DISCO_MAX_PARTS    50
#define FRUDP_MAX_PUBS 10
#define FRUDP_MAX_SUBS 10
#define FRUDP_MAX_READERS FRUDP_MAX_PUBS*10
#define FRUDP_MAX_WRITERS FRUDP_MAX_SUBS*10

#define FRUDP_MAX_TOPIC_NAME_LEN 128
#define FRUDP_MAX_TYPE_NAME_LEN  128

#define FRUDP_BUFFER_SIZE        8192
#define FRUDP_PUB_BUFLEN         2048
#define FRUDP_DISCO_TX_BUFLEN    1536

// todo: an option to generate SEDP messages on-the-fly as requested,
// rather than buffering them in precious SRAM. or maybe auto-generate the
// pub/sub messages and hold them in flash. That would be way better.
#define FRUDP_SEDP_MSG_BUFLEN    1024

#define FRUDP_SPDP_LEAVE_DURATION 60

// Delay for broadcast SPDP message (in second)
#define FRUDP_SPDP_DELAY_SEC FRUDP_SPDP_LEAVE_DURATION/3

// Delay for broadcast SEDP message (in second)
#define FRUDP_SEDP_DELAY_SEC FRUDP_SPDP_LEAVE_DURATION/6

// default multicast group is 239.255.0.1
#define FRUDP_DEFAULT_MCAST_GROUP 0xefff0001

#define FRUDP_MAGIC_WORLD 0x53505452 // RTPS in ASCII

typedef struct
{
  /** GUID prefix */
  frudp_guid_prefix_t guid_prefix;

  /** Participant ID*/
  int participant_id;

  /** Domain ID */
  uint32_t domain_id;

  /** Unicast Address */
  uint32_t unicast_addr;
} frudp_config_t;
extern frudp_config_t g_frudp_config;

//#define DEBUG

#define EXCESSIVELY_VERBOSE_MSG_RX
//#define VERBOSE_MSG_RX
//#define VERBOSE_TX_ACKNACK

//#define VERBOSE_INFO_TS
//#define VERBOSE_ACKNACK
//#define VERBOSE_HEARTBEAT
//#define VERBOSE_GAP
//#define VERBOSE_DATA

//#define VERBOSE_SEDP
//#define VERBOSE_SPDP

#ifdef __cplusplus
}
#endif
#endif // FREERTPS_CONFIG_H
