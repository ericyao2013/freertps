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

#ifndef FREERTPS_H
#define FREERTPS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "freertps/rtps/publisher.h"
#include "freertps/rtps/subscriber.h"
#include "freertps/rtps/type/qos.h"

///////////////////////////////////////////////////////////////////////////////
// Temporaly include
//#include "freertps/rtps/type/config.h"
#include "freertps/psm/system.h"
#include "freertps/psm/udp.h"
#include "freertps/rtps/type/qos.h"
#include "freertps/rtps/discovery/disco.h"
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdint.h>

// NOTE: the prefix freertps_udp_ is too long to type, so it will often
// be shortened to frudp_
typedef void (*freertps_msg_cb_t)(const void *msg, uint32_t len);

frudp_sub_t *freertps_create_sub(const char *topic_name,
                                 const char *type_name,
                                 freertps_msg_cb_t msg_cb);

// todo: come up with a better way of holding onto publishers that is
// agnostic to the physical layer
frudp_pub_t *freertps_create_pub(const char *topic_name,
                                 const char *type_name,
                                 frudp_qos_reliability_t *qos);

bool freertps_publish(frudp_pub_t *pub,
                      const uint8_t *msg,
                      const uint32_t msg_len);
//void freertps_perish_if(bool b, const char *msg);

extern bool g_freertps_init_complete;

void freertps_start(void);

void freertps_spinOnce(void);

void freertps_spin(void);

void freertps_stop(void);

#ifdef __cplusplus
}
#endif
#endif // FREERTPS_H
