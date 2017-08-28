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

#ifndef FREERTPS_SUBMSG_ID_H
#define FREERTPS_SUBMSG_ID_H

#ifdef __cplusplus
extern "C" {
#endif

#define FRUDP_SUBMSG_ID_PAD            0x01 /* Pad */
#define FRUDP_SUBMSG_ID_ACKNACK        0x06 /* AckNack */
#define FRUDP_SUBMSG_ID_HEARTBEAT      0x07 /* Heartbeat */
#define FRUDP_SUBMSG_ID_GAP            0x08 /* Gap */
#define FRUDP_SUBMSG_ID_INFO_TS        0x09 /* InfoTimestamp */
#define FRUDP_SUBMSG_ID_INFO_SRC       0x0c /* InfoSource */
#define FRUDP_SUBMSG_ID_INFO_REPLY_IP4 0x0d /* InfoReplyIp4 */
#define FRUDP_SUBMSG_ID_INFO_DEST      0x0e /* InfoDestination */
#define FRUDP_SUBMSG_ID_INFO_REPLY     0x0f /* InfoReply */
#define FRUDP_SUBMSG_ID_NACK_FRAG      0x12 /* NackFrag */
#define FRUPG_SUBMSG_ID_HEARTBEAT_FRAG 0x13 /* HeartbeatFrag */
#define FRUDP_SUBMSG_ID_DATA           0x15 /* Data */
#define FRUDP_SUBMSG_ID_DATA_FRAG      0x16 /* DataFrag */

#ifdef __cplusplus
}
#endif
#endif // FREERTPS_SUBMSG_ID_H

