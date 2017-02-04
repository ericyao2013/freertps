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

#ifndef FREERTPS_DISCO_H
#define FREERTPS_DISCO_H

// this has a bunch of discovery-related debris

#include <stdint.h>
#include "freertps/part.h"
#include "freertps/config.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define FRUDP_PID_PAD                                    0x0000     // Structure
#define FRUDP_PID_SENTINEL                               0x0001     // Structure
#define FRUDP_PID_PARTICIPANT_LEASE_DURATION             0x0002 // SPDP Duration_t
#define FRUDP_PID_PERSISTENCE                            0x0003     // Deprecated
#define FRUDP_PID_TIME_BASED_FILTER                      0x0004 // TimeBasedFilterQosPolicy
#define FRUDP_PID_TOPIC_NAME                             0x0005 // SEPD string<256>
#define FRUDP_PID_OWNERSHIP_STRENGTH                     0x0006 // OwnershipStrengthQosPolicy
#define FRUDP_PID_TYPE_NAME                              0x0007 // SEPD string<256>
#define FRUDP_PID_TYPE_CHECKSUM                          0x0008     // Deprecated
#define FRUDP_PID_TYPE2_NAME                             0x0009     // Deprecated
#define FRUDP_PID_TYPE2_CHECKSUM                         0x000a     // Deprecated
#define FRUDP_PID_METATRAFFIC_MULTICAST_IPADDRESS        0x000b // SPDP IPv4Address_t
#define FRUDP_PID_DEFAULT_UNICAST_IPADDRESS              0x000c // SPDP IPv4Address_t
#define FRUDP_PID_METATRAFFIC_UNICAST_PORT               0x000d // SPDP Port_t
#define FRUDP_PID_DEFAULT_UNICAST_PORT                   0x000e // SPDP Port_t
#define FRUDP_PID_EXPECTS_ACK                            0x0010     // Deprecated
#define FRUDP_PID_MULTICAST_IPADDRESS                    0x0011 // SPDP IPv4Address_t
#define FRUDP_PID_MANAGER_KEY                            0x0012     // Deprecated
#define FRUDP_PID_SEND_QUEUE_SIZE                        0x0013     // Deprecated
#define FRUDP_PID_RELIABILITY_ENABLED                    0x0014     // Deprecated
#define FRUDP_PID_PROTOCOL_VERSION                       0x0015 // ProtocolVersion_t
#define FRUDP_PID_VENDOR_ID                              0x0016 // VendorId_t
#define FRUDP_PID_VARGAPPS_SEQUENCE_NUMBER_LAST          0x0017     // Deprecated
#define FRUDP_PID_RECV_QUEUE_SIZE                        0x0018     // Deprecated
#define FRUDP_PID_RELIABILITY_OFFERED                    0x0019     // Deprecated
#define FRUDP_PID_RELIABILITY                            0x001a // ReliabilityQosPolicy
#define FRUDP_PID_LIVELINESS                             0x001b // SPDP LivelinessQosPolicy
#define FRUDP_PID_DURABILITY                             0x001d // DurabilityQosPolicy
#define FRUDP_PID_DURABILITY_SERVICE                     0x001e // DurabilityServiceQosPolicy
#define FRUDP_PID_OWNERSHIP                              0x001f // OwnershipQosPolicy
#define FRUDP_PID_PRESENTATION                           0x0021 // PresentationQosPolicy
#define FRUDP_PID_DEADLINE                               0x0023 // DeadlineQosPolicy
#define FRUDP_PID_DESTINATION_ORDER                      0x0025 // DestinationOrderQosPolicy
#define FRUDP_PID_LATENCY_BUDGET                         0x0027 // LatencyBudgetQosPolicy
#define FRUDP_PID_PARTITION                              0x0029 // PartitionQosPolicy
#define FRUDP_PID_LIFESPAN                               0x002b // LifespanQosPolicy
#define FRUDP_PID_USER_DATA                              0x002c // UserDataQosPolicy
#define FRUDP_PID_GROUP_DATA                             0x002d // GroupDataQosPolicy
#define FRUDP_PID_TOPIC_DATA                             0x002e // TopicDataQosPolicy
#define FRUDP_PID_UNICAST_LOCATOR                        0x002f // Locator_t
#define FRUDP_PID_MULTICAST_LOCATOR                      0x0030 // Locator_t
#define FRUDP_PID_DEFAULT_UNICAST_LOCATOR                0x0031 // SPDP Locator_t
#define FRUDP_PID_METATRAFFIC_UNICAST_LOCATOR            0x0032 // SPDP Locator_t
#define FRUDP_PID_METATRAFFIC_MULTICAST_LOCATOR          0x0033 // SPDP Locator_t
#define FRUDP_PID_PARTICIPANT_MANUAL_LIVELINESS_COUNT    0x0034 // Count_t
#define FRUDP_PID_CONTENT_FILTER_PROPERTY                0x0035 // ContentFilterProperty_t
#define FRUDP_PID_HISTORY                                0x0040 // HistoryQosPolicy
#define FRUDP_PID_RESOURCE_LIMITS                        0x0041 // ResourceLimitsQosPolicy
#define FRUDP_PID_EXPECTS_INLINE_QOS                     0x0043 // SPDP boolean
#define FRUDP_PID_PARTICIPANT_BUILTIN_ENDPOINTS          0x0044 // unsigned long
#define FRUDP_PID_METATRAFFIC_UNICAST_IPADDRESS          0x0045 // IPv4Address_t
#define FRUDP_PID_METATRAFFIC_MULTICAST_PORT             0x0046 // Port_t
#define FRUDP_PID_DEFAULT_MULTICAST_LOCATOR              0x0048 // SPDP Locator_t
#define FRUDP_PID_TRANSPORT_PRIORITY                     0x0049 // TransportPriorityQoSPolicy
#define FRUDP_PID_PARTICIPANT_GUID                       0x0050 // SPDP GUID_t
#define FRUDP_PID_PARTICIPANT_ENTITYID                   0x0051 // EntityId_t
#define FRUDP_PID_GROUP_GUID                             0x0052 // GUID_t
#define FRUDP_PID_GROUP_ENTITYID                         0x0053 // EntityId_t
#define FRUDP_PID_CONTENT_FILTER_INFO                    0x0055 // ContentFilterInfo_t
#define FRUDP_PID_COHERENT_SET                           0x0056 // SequenceNumber_t
#define FRUDP_PID_DIRECTED_WRITE                         0x0057 // sequence<GUID_t>
#define FRUDP_PID_BUILTIN_ENDPOINT_SET                   0x0058 // SPDP BuiltinEndpointSet_t
#define FRUDP_PID_PROPERTY_LIST                          0x0059 // sequence<Property_t>
#define FRUDP_PID_ENDPOINT_GUID                          0x005a // GUID_t
#define FRUDP_PID_TYPE_MAX_SIZE_SERIALIZED               0x0060 // long
#define FRUDP_PID_ORIGINAL_WRITER_INFO                   0x0061 // OriginalWriterInfo_t
#define FRUDP_PID_ENTITY_NAME                            0x0062 // EntityName_t
#define FRUDP_PID_KEY_HASH                               0x0070 // KeyHash_t
#define FRUDP_PID_STATUS_INFO                            0x0071 // StatusInfo_t
/* !SPEC */

#define FRUDP_BUILTIN_EP_PARTICIPANT_ANNOUNCER           0x00000001
#define FRUDP_BUILTIN_EP_PARTICIPANT_DETECTOR            0x00000002
#define FRUDP_BUILTIN_EP_PUBLICATION_ANNOUNCER           0x00000004
#define FRUDP_BUILTIN_EP_PUBLICATION_DETECTOR            0x00000008
#define FRUDP_BUILTIN_EP_SUBSCRIPTION_ANNOUNCER          0x00000010
#define FRUDP_BUILTIN_EP_SUBSCRIPTION_DETECTOR           0x00000020
#define FRUDP_BUILTIN_EP_PARTICIPANT_PROXY_ANNOUNCER     0x00000040
#define FRUDP_BUILTIN_EP_PARTICIPANT_PROXY_DETECTOR      0x00000080
#define FRUDP_BUILTIN_EP_PARTICIPANT_STATE_ANNOUNCER     0x00000100
#define FRUDP_BUILTIN_EP_PARTICIPANT_STATE_DETECTOR      0x00000200
#define FRUDP_BUILTIN_EP_PARTICIPANT_MESSAGE_DATA_WRITER 0x00000400
#define FRUDP_BUILTIN_EP_PARTICIPANT_MESSAGE_DATA_READER 0x00000800

void frudp_disco_init(void);
void frudp_disco_fini(void);

void frudp_disco_start(void); /// must be called to kick off discovery
void frudp_disco_tick(void);  /// must be called periodically to broadcast SPDP

extern uint8_t g_frudp_disco_tx_buf[FRUDP_DISCO_TX_BUFLEN];
extern uint16_t g_frudp_disco_tx_buf_wpos;

extern frudp_part_t g_frudp_disco_parts[FRUDP_DISCO_MAX_PARTS];
extern int g_frudp_disco_num_parts;

#ifdef __cplusplus
}
#endif
#endif // FREERTPS_DISCO_H
