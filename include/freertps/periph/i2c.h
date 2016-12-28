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

#ifndef FREERTPS_I2C_H
#define FREERTPS_I2C_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

bool i2c_init(void *i2c);

bool i2c_read(void *i2c, uint8_t device_addr, 
    uint8_t reg_addr, uint8_t len, uint8_t *buffer);

bool i2c_write(void *i2c, uint8_t device_addr, 
    uint8_t reg_addr, uint8_t len, uint8_t *buffer);

#ifdef __cplusplus
}
#endif // FREERTPS_I2C_H
#endif
