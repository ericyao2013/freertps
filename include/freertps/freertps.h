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

#include <stdio.h>

// NOTE: the prefix freertps_udp_ is too long to type, so it will often
// be shortened to frudp_

typedef void (*freertps_msg_cb_t)(const void *msg);

#include "freertps/udp.h"
#include "freertps/config.h"
#include "freertps/time.h"
#include "freertps/ports.h"
#include "freertps/locator.h"
#include "freertps/disco.h"
#include "freertps/bswap.h"
#include "freertps/system.h"
#include "freertps/pub.h"
#include "freertps/sub.h"

#ifndef Report
#define Report(...) printf(__VA_ARGS__)
#endif

#ifndef Error
#define Error(...) printf(__VA_ARGS__)
#endif

// maybe make this smarter someday
#ifdef DEBUG
#define FREERTPS_DEBUG(...) \
  do { Report("[freertps DEBUG] "); Report(__VA_ARGS__); } while (0)
#else
#define FREERTPS_DEBUG(...) ((void)0)
#endif

#define FREERTPS_INFO(...) \
  do { Report("[freertps INFO] "); Report(__VA_ARGS__); } while (0)
#define FREERTPS_WARNING(...) \
  do { Report("[freertps WARNING] "); Report(__VA_ARGS__); } while (0)
#define FREERTPS_ERROR(...) \
  do { Error("[freertps ERROR] "); Error(__VA_ARGS__); } while (0)
#define FREERTPS_FATAL(...) \
  do { Error("[freertps FATAL] "); Error(__VA_ARGS__); } while (0)

typedef union rtps_active_psms
{
    uint32_t val;
    struct rtps_active_psms_mask
    {
      uint32_t udp : 1;
      uint32_t ser : 1;
    } s;
} __attribute__((packed)) rtps_active_psms_t;

extern union rtps_active_psms g_rtps_active_psms;

void freertps_create_sub(const char *topic_name,
                         const char *type_name,
                         freertps_msg_cb_t msg_cb);

// todo: come up with a better way of holding onto publishers that is
// agnostic to the physical layer
frudp_pub_t *freertps_create_pub(const char *topic_name,
                                 const char *type_name);

bool freertps_publish(frudp_pub_t *pub,
                      const uint8_t *msg,
                      const uint32_t msg_len);
//void freertps_perish_if(bool b, const char *msg);

extern bool g_freertps_init_complete;

void freertps_start(void);

#ifdef __cplusplus
}
#endif
#endif // FREERTPS_H
