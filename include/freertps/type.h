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

#ifndef FREERTPS_TYPE_H
#define FREERTPS_TYPE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef uint32_t (*freertps_serialize_fptr_t)(
    void *msg, uint8_t *buf, uint32_t buf_size);

typedef struct freertps_type
{
  const char *rtps_typename;
  const freertps_serialize_fptr_t serialize;
} freertps_type_t;

#define FREERTPS_ARRAY(STRUCT_NAME, TYPE_NAME) \
  typedef struct freertps__ ## STRUCT_NAME ## __array \
  { \
    TYPE_NAME * data;  \
    uint32_t size;     \
    uint32_t capacity; \
  } freertps__ ## STRUCT_NAME ## __array_t;

FREERTPS_ARRAY(bool, bool);
FREERTPS_ARRAY(byte, uint8_t);
FREERTPS_ARRAY(char, int8_t);
FREERTPS_ARRAY(uint8, uint8_t);
FREERTPS_ARRAY(int8, int8_t);
FREERTPS_ARRAY(uint16, uint16_t);
FREERTPS_ARRAY(int16, int16_t);
FREERTPS_ARRAY(uint32, uint32_t);
FREERTPS_ARRAY(int32, int32_t);
FREERTPS_ARRAY(uint64, uint64_t);
FREERTPS_ARRAY(int64, int64_t);
FREERTPS_ARRAY(float32, float);
FREERTPS_ARRAY(float64, double);
FREERTPS_ARRAY(string, char *);

#ifdef __cplusplus
}
#endif
#endif // FREERTPS_TYPE_H
