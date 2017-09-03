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

#ifndef FRUDP_SUB_MESSAGE_ACKNACK_H
#define FRUDP_SUB_MESSAGE_ACKNACK_H

#include "freertps/rtps/type/entity_id.h"
#include "freertps/rtps/type/sequence_number.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct frudp_submsg_acknack
{
  frudp_eid_t    reader_id;
  frudp_eid_t    writer_id;
  frudp_sn_set_t reader_sn_state;
  // the "count" field that goes here is impossible to declare in legal C
} __attribute__((packed)) frudp_submsg_acknack_t;

#ifdef __cplusplus
}
#endif
#endif /* FRUDP_SUB_MESSAGE_ACKNACK_H */
