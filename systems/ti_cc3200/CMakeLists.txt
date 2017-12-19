## Add TI-CC3200 Common stack
include(${PROJECT_SOURCE_DIR}/systems/${SYSTEM}/cmake/ticc3200.cmake)

## Add TI-CC3200 SDK SimpleLink
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
    include
    cc3200-sdk/driverlib
    cc3200-sdk/oslib
    cc3200-sdk/simplelink/include
    cc3200-sdk/inc
)

## Add project files
add_library(freertps_system_${SYSTEM}
    src/time.c
    src/udp.c
    src/bswap.c
    src/system.c
    src/enet_mac.c
    src/enet.c)

set(SYSTEM_BONUS_LIBS "m" CACHE STRING "") # not the best variable name

set(SYSTEM_EXTRA_LIBS  "ti_cc3200_extra" CACHE STRING "extra system libs")
set(SYSTEM_NO_ROSIDL_APPS  "talker-listener_cc3200" CACHE STRING "applications for this system")

if (NOT freertps_standalone)
  ament_export_libraries(freertps_system_${SYSTEM})
endif()

add_subdirectory(extra)

## Install bin and lib
install(
        TARGETS freertps_system_${SYSTEM}
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
)
