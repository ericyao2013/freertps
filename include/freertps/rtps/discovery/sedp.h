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

#ifndef FREERTPS_SEDP_H
#define FREERTPS_SEDP_H

#include "freertps/config.h"
#include "freertps/rtps/publisher.h"
#include "freertps/rtps/subscriber.h"
#include "freertps/rtps/type/guid.h"
#include "freertps/rtps/type/part.h"
#include "freertps/log.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct sedp_topic_info
{
  frudp_guid_t guid;
  char topic_name[FRUDP_MAX_TOPIC_NAME_LEN];
  char type_name[FRUDP_MAX_TYPE_NAME_LEN];
} sedp_topic_info_t;

extern frudp_pub_t *g_sedp_sub_pub;
extern frudp_pub_t *g_sedp_pub_pub;

void frudp_sedp_init(void);
void frudp_sedp_start(void);
void frudp_sedp_tick(void);
void frudp_sedp_fini(void);

void frudp_sedp_bcast(void);

void frudp_sedp_publish_sub(frudp_sub_t *sub);
void frudp_sedp_publish_pub(frudp_pub_t *pub);

void frudp_sedp_add_builtin_endpoints(frudp_part_t *part);

void frudp_sedp_debug(void);

#ifdef VERBOSE_SEDP
#define _SEDP_INFO(...)  FREERTPS_INFO(__VA_ARGS__)
#define _SEDP_ERROR(...) FREERTPS_ERROR(__VA_ARGS__)
#define _SEDP_DEBUG(...) FREERTPS_DEBUG(__VA_ARGS__)
#define _SEDP_WARNING(...) FREERTPS_DEBUG(__VA_ARGS__)
#else
#define _SEDP_INFO(...) ((void)0)
#define _SEDP_ERROR(...) FREERTPS_ERROR(__VA_ARGS__)
#define _SEDP_DEBUG(...) ((void)0)
#define _SEDP_WARNING(...) ((void)0)
#endif

#ifdef __cplusplus
}
#endif
#endif // FREERTPS_SEDP_H
