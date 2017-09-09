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

#ifndef SYSTEMS_NATIVE_POSIX_IIO_BASE_H_
#define SYSTEMS_NATIVE_POSIX_IIO_BASE_H_

#ifdef __APPLE__
#include <iio/iio.h>
#else
#include <iio.h>
#endif

void iio_init(const char *name);
void iio_fini(void);

double get_channel_value_double(const char *name_chn, const char *name_value);
char * get_channel_value_string(const char *name_chn, const char *name_value);

#endif /* SYSTEMS_NATIVE_POSIX_IIO_BASE_H_ */
