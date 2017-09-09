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

#ifndef FREERTPS_GYRO_H
#define FREERTPS_GYRO_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void gyro_init(void);

bool gyro_poll_accels(float *xyz);

#ifdef __cplusplus
}
#endif
#endif // FREERTPS_GYRO_H
