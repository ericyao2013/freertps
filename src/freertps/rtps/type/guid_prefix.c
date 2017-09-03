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

#include "freertps/rtps/type/guid_prefix.h"

#include <stdio.h>

bool frudp_guid_prefix_identical(const frudp_guid_prefix_t * const a,
                                 const frudp_guid_prefix_t * const b)
{
//  FREERTPS_DEBUG("frudp_guid_prefix_identical(a,b)\r\n");
  for (int i = 0; i < FRUDP_GUID_PREFIX_LEN; i++)
    if (a->prefix[i] != b->prefix[i])
      return false;
  return true;
}

const char *frudp_print_guid_prefix(const frudp_guid_prefix_t *p)
{
  static char buffer[27];
  snprintf(buffer, sizeof(buffer),
           "%02x%02x%02x%02x:%02x%02x%02x%02x:%02x%02x%02x%02x",
           (unsigned)p->prefix[0],
           (unsigned)p->prefix[1],
           (unsigned)p->prefix[2],
           (unsigned)p->prefix[3],
           (unsigned)p->prefix[4],
           (unsigned)p->prefix[5],
           (unsigned)p->prefix[6],
           (unsigned)p->prefix[7],
           (unsigned)p->prefix[8],
           (unsigned)p->prefix[9],
           (unsigned)p->prefix[10],
           (unsigned)p->prefix[11]);
  return buffer;
}
