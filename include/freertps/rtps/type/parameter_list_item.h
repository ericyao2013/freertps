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

#ifndef FREERTPS_PARAMETER_LIST_ITEM_H
#define FREERTPS_PARAMETER_LIST_ITEM_H

#include "freertps/rtps/type/parameter_id.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct frudp_parameter_list_item
{
  frudp_parameterid_t pid;
  uint16_t            len;
  uint8_t             value[];
} __attribute__((packed)) frudp_parameter_list_item_t;

#define FRUDP_PLIST_ADVANCE(list_item) \
  do { \
    list_item = (frudp_parameter_list_item_t *) \
                (((uint8_t *)list_item) + 4 + list_item->len); \
  } while (0)

#ifdef __cplusplus
}
#endif
#endif /* FREERTPS_PARAMETER_LIST_ITEM_H */
