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

#include "freertps/rtps/type/guid.h"
#include "freertps/rtps/type/guid_prefix.h"

#include "freertps/psm/bswap.h"

#include <stdio.h>
#include <string.h>

const frudp_guid_t g_frudp_guid_unknown =
    { .prefix = { .prefix = {0} }, .eid = { .u = 0 } };

bool frudp_guid_identical(const frudp_guid_t * const a,
                          const frudp_guid_t * const b)
{
//  FREERTPS_DEBUG("frudp_guid_identical(a,b)\r\n");
  if (a->eid.u != b->eid.u)
    return false;

  return frudp_guid_prefix_identical(&a->prefix, &b->prefix);
}

void frudp_stuff_guid(frudp_guid_t *guid,
                      const frudp_guid_prefix_t *prefix,
                      const frudp_eid_t *id)
{
//  FREERTPS_DEBUG("frudp_stuff_guid(guid,prefix,id) make copy...\r\n");
  memcpy(&guid->prefix, prefix, sizeof(frudp_guid_prefix_t));
  guid->eid = *id;
}

const char *frudp_print_guid(const frudp_guid_t *guid)
{
  static char buffer[36];
  snprintf(buffer, sizeof(buffer),
           "%s:%08x",
           frudp_print_guid_prefix(&guid->prefix),
           (unsigned)freertps_htonl(guid->eid.u));
  return buffer;
}
