#include "freertps/periph/imu.h"
#include "freertps/config.h"

#include <stdio.h>

#ifdef SYS_FAKE_IMU

void imu_init(void)
{
  printf("native-posix fake imu init\r\n");
}

bool imu_poll_accels(float *xyz)
{
  xyz[0] = 1;
  xyz[1] = 2;
  xyz[2] = 3;
  return true;
}

#endif // SYS_FAKE_IMU
