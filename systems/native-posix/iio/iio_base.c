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

#include "iio_base.h"

#include <stdio.h>

struct iio_context *ctx;
struct iio_device  *dev;

void iio_init(const char *name)
{
  printf("native-posix iio imu init\r\n");

  printf("* Initialize context.\n");
  ctx = iio_create_local_context(); // For linux only

  printf("* Initialize device : %s\n", name);
  dev = iio_context_find_device(ctx, name);
  if (!dev) {
    perror("No device found\n");
//    shutdown();
  }
}

void iio_fini(void)
{
  printf("* Destroying context\n");
  if (ctx) { iio_context_destroy(ctx); }
}

double get_channel_value_double(const char *name_chn, const char *name_value)
{
  char buf[1024];
  static double val;

  struct iio_channel *chn = iio_device_find_channel(dev, name_chn, false);
  if (!chn) { printf("no channel found!\n"); }

  iio_channel_attr_read(chn, name_value, buf, sizeof(buf));
  val = strtod(buf, NULL);

  return val;
}
