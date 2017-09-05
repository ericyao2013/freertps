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

#ifndef FREERTPS_ACTIVE_PSMS_H
#define FREERTPS_ACTIVE_PSMS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef union rtps_active_psms
{
    uint32_t val;
    struct rtps_active_psms_mask
    {
      uint32_t udp : 1;
      uint32_t ser : 1;
    } s;
} __attribute__((packed)) rtps_active_psms_t;

extern union rtps_active_psms g_rtps_active_psms;

#ifdef __cplusplus
}
#endif
#endif /* FREERTPS_ACTIVE_PSMS_H */
