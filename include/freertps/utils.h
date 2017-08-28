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

#ifndef FRUDP_ID_H
#define FRUDP_ID_H

#include "freertps/log.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

const char *frudp_submsg(const uint16_t id);
const char *frudp_print_ip(const long ip);

#ifdef __cplusplus
}
#endif
#endif // FRUDP_ID_H
