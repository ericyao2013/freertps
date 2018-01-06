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

#ifndef FRUDP_UTILITY_H
#define FRUDP_UTILITY_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

const char *frudp_print_ip(const long ip);

void append_to_string(const char *string_append, char *string_dest);

uint16_t serialize_string_alligned(const char *string, uint8_t *buffer);
void deserialize_string_alligned(const uint8_t *buffer, uint16_t length, char *string);

void split_partition(const char * topic_name, char * topic_partition, char * topic_base_name);
void concat_partition(const char * topic_partition, const char * topic_base_name, char * topic_name);

void display_buffer(uint8_t *buffer, uint16_t length);

#ifdef __cplusplus
}
#endif
#endif // FRUDP_UTILITY_H
