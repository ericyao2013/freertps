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

#ifndef FRUDP_SUB_MESSAGE_H
#define FRUDP_SUB_MESSAGE_H

#include "freertps/rtps/type/sub_message_header.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct frudp_submsg
{
  frudp_submsg_header_t header;
  uint8_t contents[];
}  __attribute__((packed)) frudp_submsg_t;

const char *frudp_submsg(const uint16_t id);

#ifdef __cplusplus
}
#endif
#endif /* FRUDP_SUB_MESSAGE_H */
