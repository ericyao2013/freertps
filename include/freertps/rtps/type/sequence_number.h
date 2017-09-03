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

#ifndef FRUDP_SEQUENCE_NUMBER_H
#define FRUDP_SEQUENCE_NUMBER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct frudp_sn
{
  int32_t high;
  uint32_t low;
} __attribute__((packed)) frudp_sn_t; // sequence number

extern const frudp_sn_t g_frudp_sn_unknown;

typedef struct frudp_sn_set
{
  frudp_sn_t bitmap_base;
  uint32_t num_bits;
  uint32_t bitmap[];
} __attribute__((packed)) frudp_sn_set_t;

typedef struct frudp_sn_set_32bits
{
  frudp_sn_t bitmap_base;
  uint32_t num_bits;
  uint32_t bitmap;
} __attribute__((packed)) frudp_sn_set_32bits_t;

#ifdef __cplusplus
}
#endif
#endif /* FRUDP_SEQUENCE_NUMBER_H */
