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

#ifndef FREERTPS_WRITER_H
#define FREERTPS_WRITER_H

#include "freertps/config.h"
#include "freertps/rtps/type/guid.h"
#include "freertps/rtps/type/entity_id.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct frudp_writer
{
  frudp_guid_t    reader_guid;
  frudp_eid_t     writer_eid;
} __attribute__((packed)) frudp_writer_t;
// currently only supports best-effort connections

extern frudp_writer_t g_frudp_writers[FRUDP_MAX_WRITERS];
extern uint32_t g_frudp_num_writers;

void frudp_add_writer(const frudp_writer_t *writer);
void frudp_debug_writer(void);

#ifdef __cplusplus
}
#endif
#endif /* FREERTPS_WRITER_H */
