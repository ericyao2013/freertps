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

#ifndef FREERTPS_SPDP_H
#define FREERTPS_SPDP_H

#include "freertps/id.h"
#include "part.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern const frudp_eid_t g_spdp_writer_id, g_spdp_reader_id;

void frudp_spdp_init(void);
void frudp_spdp_start(void);
void frudp_spdp_tick(void);
void frudp_spdp_fini(void);
void frudp_spdp_bcast(frudp_part_t *part);

void frudp_print_participants_debug(void);

#ifdef VERBOSE_SPDP
#define _SPDP_INFO(...)  FREERTPS_INFO(__VA_ARGS__)
#define _SPDP_ERROR(...) FREERTPS_ERROR(__VA_ARGS__)
#define _SPDP_DEBUG(...) FREERTPS_DEBUG(__VA_ARGS__)
#else
#define _SPDP_INFO(...)  ((void)0)
#define _SPDP_ERROR(...) FREERTPS_ERROR(__VA_ARGS__)
#define _SPDP_DEBUG(...) ((void)0)
#endif

#ifdef __cplusplus
}
#endif
#endif // FREERTPS_SPDP_H
