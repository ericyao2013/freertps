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

#include "freertps/periph/gyro.h"
#include "freertps/config.h"
#include "iio_base.h"

#ifndef SYS_FAKE_GYRO

void gyro_init(void)
{
  iio_init("gyro_3d");
}

void gyro_fini(void)
{
  iio_fini();
}

bool gyro_poll_accels(float *xyz)
{
  xyz[0] = get_channel_value_double("anglvel_x", "raw") / 174; //16384.0f; //1000.0;
  xyz[1] = get_channel_value_double("anglvel_y", "raw") / 174; //16384.0f; //1000.0;
  xyz[2] = get_channel_value_double("anglvel_z", "raw") / 174; //16384.0f; //1000.0;
  return true;
}

#endif // SYS_FAKE_GYRO
