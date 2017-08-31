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

#include "freertps/freertps.h"
#include "freertps/rtps/type/time.h"

#include <limits.h>

fr_duration_t fr_time_diff(const fr_time_t *end, const fr_time_t *start)
{
  // FUTURE: this can probably be simplified.
  fr_duration_t d;
  if (end->seconds >= start->seconds)
  {
    d.fraction = end->fraction - start->fraction;
    if (end->fraction >= start->fraction)
      d.seconds = end->seconds - start->seconds;
    else
      d.seconds = end->seconds - start->seconds - 1;
  }
  else
  {
    d.fraction = start->fraction - end->fraction;
    if (end->fraction > start->fraction)
      d.seconds  = end->seconds - start->seconds + 1;
    else
      d.seconds  = end->seconds - start->seconds;
  }
  return d;
}

double fr_time_double(const fr_time_t *t)
{
  if (t->seconds >= 0)
    return t->seconds + t->fraction / (double)UINT_MAX;
  else
  {
    FREERTPS_ERROR("invalid fr_time: (%d, %u)\n", 
                   (int)t->seconds, (unsigned)t->fraction);
    return 0;
  }
}

double fr_duration_double(const fr_duration_t *t)
{
  if (t->seconds >= 0)
    return t->seconds + t->fraction / (double)UINT_MAX;
  else
    return t->seconds - t->fraction / (double)UINT_MAX;
}

double fr_time_now_double(void)
{
  fr_time_t t = fr_time_now();
  return fr_duration_double(&t);
}
