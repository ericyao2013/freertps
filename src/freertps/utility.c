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
