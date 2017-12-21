#include "freertps/rtps/type/time.h"
#include <time.h>
#include "metal/systime.h"

fr_time_t fr_time_now(void)
{
  fr_time_t now;
  uint32_t t = systime_usecs();
  // todo: provide faster macros for this which lose precision
  const uint32_t frac_sec_lcm = t * 1000 * 8388608;
  const uint32_t frac_sec = frac_sec_lcm / 1953125;
  now.seconds = 1436564555 + t / 1000000; // NOT SMART! fix this
  now.fraction = frac_sec;
  return now;
}
