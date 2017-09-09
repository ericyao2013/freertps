include(${PROJECT_SOURCE_DIR}/systems/${SYSTEM}/cmake/ticc3200.cmake)

set(CC3200_SDK_DIR "${CMAKE_SOURCE_DIR}/systems/${SYSTEM}/cc3200-sdk")
set(CC3200_SDK_VERSION "1.0.0")

if (NOT EXISTS ${CC3200_SDK_DIR})
        include(cmake/modules/DownloadProject.cmake)
        download_project(PROJ       cc3200
                SOURCE_DIR          ${CC3200_SDK_DIR}
                GIT_REPOSITORY      https://github.com/rosalfred/cc3200_sdk.git
                GIT_TAG             ${CC3200_SDK_VERSION}
                UPDATE_DISCONNECTED 1
                QUIET
        )
endif()

include_directories(
    common
    cc3200-sdk/driverlib
    cc3200-sdk/oslib
    cc3200-sdk/simplelink
    cc3200-sdk/simplelink/include
    cc3200-sdk/simplelink/source
    cc3200-sdk/third_party/FreeRTOS/source
    cc3200-sdk/third_party/FreeRTOS/source/include
    cc3200-sdk/third_party/FreeRTOS/source/portable/GCC/ARM_CM4
    cc3200-sdk/middleware/framework/pm
    cc3200-sdk/middleware/framework/timer
    cc3200-sdk/middleware/driver/
    cc3200-sdk/middleware/driver/hal
    cc3200-sdk/middleware/soc
    cc3200-sdk/middleware
    cc3200-sdk/simplelink_extlib/include
    cc3200-sdk/simplelink_extlib/flc
    cc3200-sdk/simplelink_extlib/ota
    #cc3200-sdk/example/common
    cc3200-sdk/inc
)

# Add source files
#FILE(GLOB sources
#    cc3200-sdk/driverlib/*.c
#    cc3200-sdk/oslib/*.c
#    cc3200-sdk/simplelink/cc_pal.c cc3200-sdk/simplelink/source/*.c
#    cc3200-sdk/third_party_FreeRTOS/src/*.c cc3200-sdk/third_party_FreeRTOS/portable/GCC/ARM_CM4/*.c cc3200-sdk/third_party_FreeRTOS/portable/MemMang/heap_3.c
#    cc3200-sdk/middleware/driver/*.c cc3200-sdk/middleware/driver/harl/*.c cc3200-sdk/middleware/framework/pm/*.c cc3200-sdk/middleware/framework/timer/*.c cc3200-sdk/middleware/soc/*.c
#    cc3200-sdk/simplelink_extlib/flc/*.c cc3200-sdk/simplelink_extlib/ota/*.c
#    cc3200-sdk/example/common/*.c
#)

add_library(freertps_system_${SYSTEM}
    src/time.c
    src/udp.c
    src/bswap.c
    src/system.c
    src/enet_mac.c
    src/enet.c)

target_link_libraries(freertps_system_${SYSTEM} freertps)

set(SYSTEM_BONUS_LIBS "m" CACHE STRING "") # not the best variable name

set(SYSTEM_EXTRA_LIBS  "ti_cc3200_extra" CACHE STRING "extra system libs")
set(SYSTEM_NO_ROSIDL_APPS  "talker-listener_cc3200" CACHE STRING "applications for this system")

if (NOT freertps_standalone)
  ament_export_libraries(freertps_system_${SYSTEM})
endif()

add_subdirectory(extra)

install(
  	TARGETS freertps_system_${SYSTEM}
  	ARCHIVE DESTINATION lib
  	LIBRARY DESTINATION lib
  	RUNTIME DESTINATION bin
)
