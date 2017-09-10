set(SYS_FAKE_IMU ON)
set(SYS_FAKE_GYRO ON)
set(SYS_FAKE_MAGN ON)

set(SOURCE        time.c
                  udp.c
                  bswap.c
                  system.c
                  fake_imu.c
                  fake_led.c
                  ser.c)

if(NOT SYS_FAKE_IMU AND NOT SYS_FAKE_GYRO AND NOT SYS_FAKE_MAGN)
find_library(iio /usr/lib/x86_64-linux-gnu/libiio.so.0)
set(SOURCE        ${SOURCE}
                  iio/iio_base.c
                  iio/als.c
                  iio/gyro.c
                  iio/imu.c
                  iio/magn.c)
endif()

add_library(freertps_system_native-posix  ${SOURCE})

target_link_libraries(freertps_system_native-posix freertps)

if(NOT SYS_FAKE_IMU AND NOT SYS_FAKE_GYRO AND NOT SYS_FAKE_MAGN)
target_link_libraries(freertps_system_native-posix iio)
endif()

set(SYSTEM_EXTRA_LIBS  "" CACHE STRING "")
set(SYSTEM_BONUS_LIBS "m" CACHE STRING "") # not the best variable name
set(SYSTEM_APPS  "led talker listener imu" CACHE STRING "")
set(SYSTEM_NO_ROSIDL_APPS "talker_no_rosidl listener_no_rosidl standalone_listen_for_n standalone_talk_n" CACHE STRING "")
install(
  TARGETS freertps_system_native-posix
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)
