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

#ifndef FREERTPS_EID_H
#define FREERTPS_EID_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FRUDP_ENTITY_KIND_USER_WRITER_WITH_KEY 0x02
#define FRUDP_ENTITY_KIND_USER_WRITER_NO_KEY   0x03
#define FRUDP_ENTITY_KIND_USER_READER_NO_KEY   0x04
#define FRUDP_ENTITY_KIND_USER_READER_WITH_KEY 0x07

typedef union
{
  struct
  {
    uint8_t key[3];
    uint8_t kind;
  } s;
  uint32_t u;
} __attribute__((packed)) frudp_eid_t; // entity ID

frudp_eid_t frudp_create_user_id(const uint8_t entity_kind);

extern const frudp_eid_t g_frudp_eid_unknown;

#ifdef __cplusplus
}
#endif
#endif // FREERTPS_EID_H
