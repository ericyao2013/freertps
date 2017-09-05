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

#include "freertps/config.h"
#include "freertps/rtps/reader.h"
#include "freertps/rtps/type/entity_id.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct frudp_sub
{
  const char *topic_name;
  const char *type_name;
  frudp_eid_t reader_eid;
  frudp_rx_data_cb_t data_cb;
  freertps_msg_cb_t msg_cb;
  bool reliable;
} __attribute__((packed)) frudp_sub_t;

extern frudp_sub_t g_frudp_subs[FRUDP_MAX_SUBS];
extern uint32_t g_frudp_num_subs;

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

/**
 *
 */
void frudp_add_sub(const frudp_sub_t *s);

#ifdef __cplusplus
}
#endif
#endif // FREERTPS_SUBSCRIPTION_H
