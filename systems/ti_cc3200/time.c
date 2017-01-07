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
#include "hw_memmap.h"
#include "simplelink.h"
#include "freertps/time.h"

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
  fr_time_t now = TIME_INVALID;
  uint64_t ulSecs; // Time in Second Unit
  uint16_t usMsec; // Precision of Time in Milisecond Unit

  PRCMRTCGet(&ulSecs, &usMsec);

  uint64_t time_frac = (ulSecs * 1000 + usMsec); // Time in Milisencond Unit

  now.seconds = ulSecs;
  now.fraction = time_frac;
  return now;
}
