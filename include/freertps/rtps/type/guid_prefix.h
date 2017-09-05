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

#ifndef FREERTPS_GUID_PREFIX_H
#define FREERTPS_GUID_PREFIX_H

#include "freertps/rtps/type/entity_id.h"

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FRUDP_GUID_PREFIX_LEN 12

typedef struct frudp_guid_prefix
{
  uint8_t prefix[FRUDP_GUID_PREFIX_LEN];
} __attribute__((packed)) frudp_guid_prefix_t;

bool frudp_guid_prefix_identical(const frudp_guid_prefix_t * const a,
                                 const frudp_guid_prefix_t * const b);

const char *frudp_print_guid_prefix(const frudp_guid_prefix_t *guid_prefix);

#ifdef __cplusplus
}
#endif
#endif // FREERTPS_GUID_PREFIX_H
