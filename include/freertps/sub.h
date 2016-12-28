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

#ifndef FREERTPS_SUBSCRIPTION_H
#define FREERTPS_SUBSCRIPTION_H

#include "freertps/id.h"
#include "freertps/udp.h"
#include "freertps/freertps.h"
#include "freertps/config.h"

#ifdef __cplusplus
extern "C"
{
#endif

// create userland UDP subscriptions. people should call the
// freertps_create_subscription() from userland code though, to be agnostic
// to the physical layer

// could be dangerous to hold onto string pointers... they need to be
// stored in the caller's constant memory. maybe revisit this at some point,
// with a #define switch somewhere to have it use more memory for string
// buffers, etc.

void frudp_add_user_sub(const char *topic_name,
                        const char *type_name,
                        freertps_msg_cb_t msg_cb);

// this is the private subscribe function used internally... should be hidden
// eventually.
/*
bool frudp_subscribe(const frudp_entity_id_t reader_id,
                     const frudp_entity_id_t writer_id,
                     const frudp_rx_data_cb_t data_cb,
                     const freertps_msg_cb_t msg_cb);
*/

typedef struct
{
  const char *topic_name;
  const char *type_name;
  frudp_eid_t reader_eid;
  frudp_rx_data_cb_t data_cb;
  freertps_msg_cb_t msg_cb;
  bool reliable;
} frudp_sub_t;

void frudp_add_sub(const frudp_sub_t *s);

extern frudp_sub_t g_frudp_subs[FRUDP_MAX_SUBS];
extern uint32_t g_frudp_num_subs;

typedef struct
{
  bool reliable;
  frudp_guid_t writer_guid;
  frudp_eid_t reader_eid;
  frudp_sn_t max_rx_sn;
  frudp_rx_data_cb_t data_cb;
  freertps_msg_cb_t msg_cb;
} frudp_reader_t;

// not great to have these freely available. someday hide these.
extern frudp_reader_t g_frudp_readers[FRUDP_MAX_READERS];
extern uint32_t g_frudp_num_readers;

void frudp_add_reader(const frudp_reader_t *reader);

void frudp_print_readers(void);

#ifdef __cplusplus
}
#endif
#endif // FREERTPS_SUBSCRIPTION_H
