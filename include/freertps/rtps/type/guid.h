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

#ifndef FREERTPS_GUID_H
#define FREERTPS_GUID_H

#include "freertps/rtps/type/entity_id.h"
#include "freertps/rtps/type/guid_prefix.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FRUDP_GUID_PREFIX_LEN 12

typedef struct frudp_guid
{
  frudp_guid_prefix_t    prefix;
  frudp_eid_t            eid;
} __attribute__((packed)) frudp_guid_t;

bool frudp_guid_identical(const frudp_guid_t * const a,
                          const frudp_guid_t * const b);

void frudp_stuff_guid(frudp_guid_t *guid,
                      const frudp_guid_prefix_t *prefix,
                      const frudp_eid_t *id);

const char *frudp_print_guid(const frudp_guid_t *guid);

extern const frudp_guid_t g_frudp_guid_unknown;

#ifdef __cplusplus
}
#endif
#endif // FREERTPS_GUID_H
