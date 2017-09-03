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

#ifndef FRUDP_RECEIVER_STATE_H
#define FRUDP_RECEIVER_STATE_H

#include "freertps/rtps/type/protocol_version.h"
#include "freertps/rtps/type/vendor_id.h"
#include "freertps/rtps/type/guid_prefix.h"
#include "freertps/rtps/type/time.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct frudp_receiver_state
{
  frudp_pver_t        src_pver;
  frudp_vid_t         src_vid;
  frudp_guid_prefix_t src_guid_prefix;
  frudp_guid_prefix_t dst_guid_prefix;
  bool                have_timestamp;
  fr_time_t           timestamp;
} __attribute__((packed)) frudp_receiver_state_t;

#ifdef __cplusplus
}
#endif
#endif /* FRUDP_RECEIVER_STATE_H */
