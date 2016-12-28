add_library(freertps_system_ti_cc3200
            bswap.c
            enet_mac.c
            enet.c
            system.c
            time.c
            timer.c
            udp.c)
set(SYSTEM_EXTRA_LIBS  "metal_common" CACHE STRING "extra system libs")
#set(SYSTEM_APPS "usb_test cam" CACHE STRING "applications")
#set(SYSTEM_NO_ROSIDL_APPS "blink talker_no_rosidl listener_no_rosidl" CACHE STRING "")
#set(SYSTEM_APPS "usb_test talker listener enet_test cam" CACHE STRING "applications")
#set(SYSTEM_APPS "blink hello_world talker listener systime_test enet_test" CACHE STRING "applications")
#set(SYSTEM_APPS  "listener talker talker_stm32_timer cam_dm_130" CACHE STRING "applications for this system")
