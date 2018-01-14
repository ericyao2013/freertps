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

#ifndef FREERTPS_CONFIG_DEFAULT_H
#define FREERTPS_CONFIG_DEFAULT_H

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

#define FRUDP_MAX_TOPIC_NAME_LEN 128 // The length of the DDS topic must not exceed 256 characters. RTI Connext limit to 248.
#define FRUDP_MAX_TYPE_NAME_LEN  128
#define FRUDP_MAX_PARTITION_NAME_LEN 128 // The actual length of a partition field may be limited to 256 characters. (RTI limit to 248.

#define FRUDP_BUFFER_SIZE        8192
#define FRUDP_PUB_BUFLEN         2048
#define FRUDP_DISCO_TX_BUFLEN    1536

// todo: an option to generate SEDP messages on-the-fly as requested,
// rather than buffering them in precious SRAM. or maybe auto-generate the
// pub/sub messages and hold them in flash. That would be way better.
#define FRUDP_SEDP_MSG_BUFLEN    1024

// Delay of leave bail of SPDP (in second) [Default : 100]
#define FRUDP_SPDP_LEAVE_DURATION 100

// Delay for broadcast SPDP message (in second) [Default : 30]
#define FRUDP_SPDP_DELAY_SEC 30

// Delay for broadcast SEDP message (in second)
#define FRUDP_SEDP_DELAY_SEC FRUDP_SPDP_LEAVE_DURATION

// default multicast group is 239.255.0.1
#define FRUDP_DEFAULT_MCAST_GROUP 0xefff0001

#define FRUDP_MAGIC_WORLD 0x53505452 // RTPS in ASCII


#ifdef VERBOSE_ENABLE
#define DEBUG

#define EXCESSIVELY_VERBOSE_MSG_RX
#define VERBOSE_MSG_RX
#define VERBOSE_TX_ACKNACK

#define VERBOSE_INFO_TS
#define VERBOSE_INFO_DEST
#define VERBOSE_ACKNACK
#define VERBOSE_HEARTBEAT
#define VERBOSE_GAP
#define VERBOSE_DATA

#define VERBOSE_SEDP
#define VERBOSE_SPDP

#define SYS_FAKE_IMU
#define SYS_FAKE_GYRO
#define SYS_FAKE_MAGN
#endif

//#define VERBOSE_MSG_SERIALIZATION

#ifdef __cplusplus
}
#endif
#endif // FREERTPS_CONFIG_H
