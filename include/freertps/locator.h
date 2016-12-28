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

#ifndef FREERTPS_LOCATOR_H
#define FREERTPS_LOCATOR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define FRUDP_LOCATOR_KIND_INVALID -1
#define FRUDP_LOCATOR_KIND_RESERVED 0
#define FRUDP_LOCATOR_KIND_UDPV4    1
#define FRUDP_LOCATOR_KIND_UDPV6    2

typedef struct
{
  int32_t kind;
  uint32_t port;
  union
  {
    uint8_t raw[16];
    struct
    {
      uint8_t zeros[12];
      uint32_t addr;
    } udp4;
  } addr; 
} __attribute__((packed)) frudp_locator_t;

#ifdef __cplusplus
}
#endif
#endif // FREERTPS_LOCATOR_H
