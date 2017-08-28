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

#ifndef FRUDP_QOS_H
#define FRUDP_QOS_H

#include "freertps/psm/udp.h"

#ifdef __cplusplus
extern "C"
{
#endif

///////////////////////////////////////////////////////////////
#define FRUDP_QOS_RELIABILITY_KIND_BEST_EFFORT 1
#define FRUDP_QOS_RELIABILITY_KIND_RELIABLE    2

typedef struct
{
  uint32_t kind;
  frudp_duration_t max_blocking_time;
} __attribute__((packed)) frudp_qos_reliability_t;

///////////////////////////////////////////////////////////////
#define FRUDP_QOS_HISTORY_KIND_KEEP_LAST 0
#define FRUDP_QOS_HISTORY_KIND_KEEP_ALL  1

typedef struct
{
  uint32_t kind;
  uint32_t depth;
} __attribute__((packed)) frudp_qos_history_t;

///////////////////////////////////////////////////////////////
#define  FRUDP_QOS_PRESENTATION_SCOPE_TOPIC 1

typedef struct
{
  uint32_t scope;
  uint16_t coherent_access;
  uint16_t ordered_access;
} __attribute__((packed)) frudp_qos_presentation_t;

#ifdef __cplusplus
}
#endif
#endif // FRUDP_QOS_H
