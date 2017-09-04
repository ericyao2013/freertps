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

#ifndef FREERTPS_READER_H
#define FREERTPS_READER_H

#include "freertps/config.h"
#include "freertps/rtps/type/guid.h"
#include "freertps/rtps/type/sequence_number.h"
#include "freertps/rtps/type/sub_message.h"
#include "freertps/rtps/type/receiver_state.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*freertps_msg_cb_t)(const void *msg, uint32_t len);
typedef void (*frudp_rx_data_cb_t)(frudp_receiver_state_t *rcvr,
                                   const frudp_submsg_t *submsg,
                                   const uint16_t scheme,
                                   const uint8_t *data);

typedef struct frudp_reader
{
  bool reliable;
  frudp_guid_t writer_guid;
  frudp_eid_t reader_eid;
  frudp_sn_t max_rx_sn;
  frudp_rx_data_cb_t data_cb;
  freertps_msg_cb_t msg_cb;
} __attribute__((packed)) frudp_reader_t;

// not great to have these freely available. someday hide these.
extern frudp_reader_t g_frudp_readers[FRUDP_MAX_READERS];
extern uint32_t g_frudp_num_readers;

void frudp_add_reader(const frudp_reader_t *reader);
void frudp_debug_readers(void);

#ifdef __cplusplus
}
#endif
#endif /* FREERTPS_READER_H */
