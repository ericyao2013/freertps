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

#ifndef FREERTPS_CONFIG_H
#define FREERTPS_CONFIG_H

#include "freertps/rtps/type/guid_prefix.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
  /** GUID prefix */
  frudp_guid_prefix_t guid_prefix;

  /** Participant ID*/
  uint32_t participant_id;

  /** Domain ID */
  uint32_t domain_id;

  /** Unicast Address */
  uint32_t unicast_addr;
} __attribute__((packed)) frudp_config_t;

frudp_config_t g_frudp_config;

#ifdef __cplusplus
}
#endif
#endif // FREERTPS_CONFIG_H
