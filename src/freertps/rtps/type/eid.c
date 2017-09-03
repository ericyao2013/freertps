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

#include "freertps/rtps/type/eid.h"
#include "freertps/log.h"

static unsigned g_frudp_next_user_eid = 1;

frudp_eid_t frudp_create_user_id(const uint8_t entity_kind)
{
  FREERTPS_DEBUG("frudp_create_user_id()\r\n");

  frudp_eid_t eid;
  eid.s.kind = entity_kind; // entity kind must be set by caller of this function must be overwritten by FRUDP_ENTITY_KIND_USER_READER_NO_KEY; // has key? dunno
  eid.s.key[0] = 0;
  eid.s.key[1] = 0; // todo: >8 bit ID's
  eid.s.key[2] = g_frudp_next_user_eid++;
  return eid;
}
