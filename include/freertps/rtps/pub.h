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

#ifndef FREERTPS_PUB_H
#define FREERTPS_PUB_H

#include "freertps/config.h"
#include "freertps/rtps/type/entity_id.h"
#include "freertps/rtps/type/part.h"
#include "freertps/rtps/type/qos.h"
#include "freertps/rtps/type/sequence_number.h"
#include "freertps/rtps/type/sub_message_data.h"
#include "freertps/rtps/type/sub_message_acknack.h"
#include "freertps/psm/udp.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct frudp_pub
{
  const char           *topic_name;
  const char           *type_name;
  frudp_qos_reliability_t qos;
  frudp_eid_t           writer_eid;
  frudp_sn_t            max_tx_sn_avail;
  frudp_sn_t            min_tx_sn_avail;
  uint32_t              num_data_submsgs;
  frudp_submsg_data_t **data_submsgs;
  uint32_t              next_submsg_idx;
  frudp_sn_t            next_sn;
  bool                  reliable;
} frudp_pub_t;

extern frudp_pub_t g_frudp_pubs[FRUDP_MAX_PUBS];
extern uint32_t g_frudp_num_pubs;

//////////////////////////////////////////////////////////////

typedef struct
{
  frudp_guid_t reader_guid;
  frudp_eid_t writer_eid;
} frudp_writer_t; // currently only supports best-effort connections

extern frudp_writer_t g_frudp_writers[FRUDP_MAX_WRITERS];
extern uint32_t g_frudp_num_writers;

//////////////////////////////////////////////////////////////

frudp_pub_t *frudp_create_pub(const char *topic_name,
                              const char *type_name,
                              frudp_qos_reliability_t *qos,
                              const frudp_eid_t writer_id,
                              frudp_submsg_data_t **data_submsgs,
                              const uint32_t num_data_submsgs);

void frudp_publish(frudp_pub_t *publication, 
                   frudp_submsg_data_t *submsg);

bool frudp_publish_user_msg(frudp_pub_t *publication,
    const uint8_t *msg, const uint32_t msg_len);

bool frudp_publish_user_msg_frag(
    frudp_pub_t *publication,
    const uint32_t frag_num, 
    const uint8_t *frag, 
    const uint32_t frag_len,
    const uint32_t frag_used_len,
    const uint32_t msg_len);

frudp_pub_t *frudp_pub_from_writer_id(const frudp_eid_t id);

void frudp_pub_rx_acknack(frudp_pub_t *pub,
                          frudp_submsg_acknack_t *acknack,
                          frudp_guid_prefix_t *guid_prefix);

frudp_pub_t *frudp_create_user_pub(const char *topic_name,
                                   const char *type_name,
                                   frudp_qos_reliability_t *qos);

void frudp_add_writer(const frudp_writer_t *writer);

void frudp_send_sedp_msgs(frudp_part_t *part);

#ifdef __cplusplus
}
#endif
#endif // FREERTPS_PUB_H
