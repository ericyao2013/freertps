// Copyright 2016 Mickael Gaillard <mick.gaillard@gmail.com>.
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

#include <time.h>
#include "simplelink.h"
#include "freertps/time.h"
#include "hw_memmap.h"

uint32_t systime_usecs()
{
    unsigned long ulSecs;
    unsigned short usMsec;

    PRCMRTCGet(&ulSecs, &usMsec);
    uint32_t res = (ulSecs * 1000 + usMsec) * 1000;

    return res;
}

fr_time_t fr_time_now(void)
{
  fr_time_t now;
  uint32_t t = systime_usecs();
  // todo: provide faster macros for this which lose precision
  const uint64_t frac_sec_lcm = (uint64_t)t * 1000 * 8388608;
  const uint64_t frac_sec = frac_sec_lcm / 1953125;
  now.seconds = 1436564555 + t / 1000000; // NOT SMART! fix this
  now.fraction = frac_sec;
  return now;
}
