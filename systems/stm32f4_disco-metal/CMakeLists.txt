include_directories(${PROJECT_SOURCE_DIR}/systems/stm32_common)
include_directories(.)
add_library(freertps_system_stm32f4_disco-metal console.c 
                                led.c 
                                systime.c
                                flash.c
                                imu.c
                                enet_init_pins.c
                                usb_fs_init_pins.c
                                ../stm32_common/stm32f4_vectors.c)
target_link_libraries(freertps_system_stm32f4_disco-metal freertps)
set(SYSTEM_EXTRA_LIBS  "stm32_common metal_common" CACHE STRING "extra system libs")
set(SYSTEM_APPS  "listener talker talker_stm32_timer imu led blink hello_world cpp_test" CACHE STRING "applications for this system")
set(SYSTEM_NO_ROSIDL_APPS "talker_no_rosidl listener_no_rosidl" CACHE STRING "")
if (NOT freertps_standalone)
  ament_export_libraries(freertps_system_stm32f4_disco-metal)
endif()
install(
  TARGETS freertps_system_stm32f4_disco-metal
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)
