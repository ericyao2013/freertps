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

#include "freertps/utility.h"
#include "freertps/psm/bswap.h"
#include "freertps/rtps/constant/sub_message_id.h"

#include <stdio.h>

/** Display IP
 * @param ip in normal mode.
 */
const char *frudp_print_ip(const long ip)
{
  static char buffer[16];
  snprintf(buffer, sizeof(buffer),
           "%03d.%03d.%03d.%03d",
           (unsigned int)FREERTPS_IPV4_BYTE(ip,3),
           (unsigned int)FREERTPS_IPV4_BYTE(ip,2),
           (unsigned int)FREERTPS_IPV4_BYTE(ip,1),
           (unsigned int)FREERTPS_IPV4_BYTE(ip,0));
  return buffer;
}

const char * append_to_string(const char *string_append, char *string_dest)
{
  char tmp[strlen(string_dest)];
  memcpy(tmp, string_dest, strlen(string_dest));

  memcpy(string_dest, string_append, strlen(string_append));
  memmove(string_dest+strlen(string_append),tmp,sizeof(tmp));
  string_dest[sizeof(tmp) + strlen(string_append)] = 0;
}

uint16_t serialize_string_alligned(const char *string, uint8_t *buffer)
{
  int string_len = string ? strlen(string) : 0;

  // Second copy
  memcpy(buffer, string, string_len + 1);

  return (string_len + 4) & ~0x3; // params must be 32-bit aligned
}

void deserialize_string_alligned(uint8_t *buffer, uint16_t length, char *string)
{
//  const char *string = malloc(sizeof(char) * (length + 1));
  memcpy(string, buffer, length + 1);
//  return string;
}

void display_buffer(uint8_t *buffer, uint16_t length)
{
  printf("size = %d\n", length);
  printf("|           |           |           |           |           |           |           |           |\n");

  for(int i = 0 ; i < length ; ++i)
  {
    printf("%02x ", buffer[i]);

//    if (i != 0 && i % 9 == 0)
//    {
//    	printf("\n");
//    }
  }
  printf("\n");
}





