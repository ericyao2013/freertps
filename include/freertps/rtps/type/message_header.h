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

#ifndef FRUDP_MESSAGE_HEADER_H
#define FRUDP_MESSAGE_HEADER_H

#include "freertps/rtps/type/protocol_version.h"
#include "freertps/rtps/type/vendor_id.h"
#include "freertps/rtps/type/guid_prefix.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct frudp_header
{
  uint32_t magic_word; // RTPS in ASCII
  frudp_pver_t pver;   // protocol version
  frudp_vid_t  vid;    // vendor ID
  frudp_guid_prefix_t guid_prefix;
} __attribute__((packed)) frudp_header_t;

#ifdef __cplusplus
}
#endif
#endif /* FRUDP_MESSAGE_HEADER_H */
