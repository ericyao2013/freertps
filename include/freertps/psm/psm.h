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

#ifndef FREERTPS_PSM_H
#define FREERTPS_PSM_H

#include "freertps/rtps/publisher.h"
#include "freertps/freertps.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef bool (*rtps_psm_init_func_t)(void);
typedef void (*rtps_psm_disco_func_t)(void);
typedef frudp_pub_t *(*rtps_psm_create_pub_func_t)(
    const char *topic_name, const char *type_name);
typedef void (*rtps_psm_create_sub_func_t)(
    const char *topic_name, const char *type_name, freertps_msg_cb_t msg_cb);
typedef void (*rtps_psm_pub_func_t)(
    void *pub, const uint8_t *msg, const uint32_t msg_len);

struct rtps_psm
{
  rtps_psm_init_func_t       init;
  rtps_psm_disco_func_t      disco;
  rtps_psm_create_pub_func_t create_pub;
  rtps_psm_create_sub_func_t create_sub;
  rtps_psm_pub_func_t        pub;
};

extern struct rtps_psm g_rtps_psm;

#ifdef __cplusplus
}
#endif
#endif // FREERTPS_PSM_H
