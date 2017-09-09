add_definitions(-DUSE_FREERTOS)

include_directories(
    ${CMAKE_SOURCE_DIR}/systems/ti_cc3200/common
    ${CMAKE_SOURCE_DIR}/systems/ti_cc3200/cc3200-sdk/driverlib
    ${CMAKE_SOURCE_DIR}/systems/ti_cc3200/cc3200-sdk/oslib
    ${CMAKE_SOURCE_DIR}/systems/ti_cc3200/cc3200-sdk/simplelink
    ${CMAKE_SOURCE_DIR}/systems/ti_cc3200/cc3200-sdk/simplelink/include
    ${CMAKE_SOURCE_DIR}/systems/ti_cc3200/cc3200-sdk/simplelink/source
    ${CMAKE_SOURCE_DIR}/systems/ti_cc3200/cc3200-sdk/third_party/FreeRTOS/source
    ${CMAKE_SOURCE_DIR}/systems/ti_cc3200/cc3200-sdk/third_party/FreeRTOS/source/include
    ${CMAKE_SOURCE_DIR}/systems/ti_cc3200/cc3200-sdk/third_party/FreeRTOS/source/portable/GCC/ARM_CM4
    ${CMAKE_SOURCE_DIR}/systems/ti_cc3200/cc3200-sdk/middleware/framework/pm
    ${CMAKE_SOURCE_DIR}/systems/ti_cc3200/cc3200-sdk/middleware/framework/timer
    ${CMAKE_SOURCE_DIR}/systems/ti_cc3200/cc3200-sdk/middleware/driver/
    ${CMAKE_SOURCE_DIR}/systems/ti_cc3200/cc3200-sdk/middleware/driver/hal
    ${CMAKE_SOURCE_DIR}/systems/ti_cc3200/cc3200-sdk/middleware/soc
    ${CMAKE_SOURCE_DIR}/systems/ti_cc3200/cc3200-sdk/middleware
    ${CMAKE_SOURCE_DIR}/systems/ti_cc3200/cc3200-sdk/simplelink_extlib/include
    ${CMAKE_SOURCE_DIR}/systems/ti_cc3200/cc3200-sdk/simplelink_extlib/flc
    ${CMAKE_SOURCE_DIR}/systems/ti_cc3200/cc3200-sdk/simplelink_extlib/ota
    #cc3200-sdk/example/common
    ${CMAKE_SOURCE_DIR}/systems/ti_cc3200/cc3200-sdk/inc
)

set(CMAKE_EXECUTABLE_SUFFIX .elf)
set(CMAKE_EXE_LINKER_FLAGS "-T ${PROJECT_SOURCE_DIR}/systems/ti_cc3200/linker.ld -Xlinker --gc-sections")

freertps_add_executable(talker-listener_cc3200 talker-listener_cc3200.c)
