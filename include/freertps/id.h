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

#ifndef FRUDP_ID_H
#define FRUDP_ID_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
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

extern const frudp_eid_t g_frudp_eid_unknown;

#define FRUDP_GUID_PREFIX_LEN 12
typedef struct
{
  uint8_t prefix[FRUDP_GUID_PREFIX_LEN];
} frudp_guid_prefix_t;

typedef struct
{
  frudp_guid_prefix_t prefix;
  frudp_eid_t eid;
} __attribute__((packed)) frudp_guid_t;
extern const frudp_guid_t g_frudp_guid_unknown;

bool frudp_guid_prefix_identical(frudp_guid_prefix_t * const a,
                                 frudp_guid_prefix_t * const b);

bool frudp_guid_identical(const frudp_guid_t * const a,
                          const frudp_guid_t * const b);

void frudp_stuff_guid(frudp_guid_t *guid,
                      const frudp_guid_prefix_t *prefix,
                      const frudp_eid_t *id);

/////////////////////////////////////////////////////////////////////////
// VENDOR ID STUFF
// for now let's pretend that our vendor ID is 11311 in hex
#define FREERTPS_VENDOR_ID 0x2C2F
typedef uint16_t frudp_vid_t;

const char *frudp_submsg(const uint16_t id);
const char *frudp_vendor(const frudp_vid_t vid);
const char *frudp_print_guid_prefix(const frudp_guid_prefix_t *guid_prefix);
const char *frudp_print_guid(const frudp_guid_t *guid);
const char *frudp_print_ip(const long ip);

frudp_eid_t frudp_create_user_id(const uint8_t entity_kind);

#ifdef __cplusplus
}
#endif
#endif // FRUDP_ID_H
