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

#ifndef FREERTPS_BSWAP_H
#define FREERTPS_BSWAP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

// todo: something clever with inlining someday, if this matters
uint32_t freertps_htonl(uint32_t u);
uint16_t freertps_htons(uint16_t u);
uint32_t freertps_ntohl(uint32_t u);
uint16_t freertps_ntohs(uint16_t u);

#ifdef __cplusplus
}
#endif
#endif // FREERTPS_BSWAP_H
