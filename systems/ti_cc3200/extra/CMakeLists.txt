add_definitions(-Dgcc)

include_directories(
    ../cc3200-sdk
    ../cc3200-sdk/inc
    ../cc3200-sdk/driverlib
    ../cc3200-sdk/oslib
    ../cc3200-sdk/simplelink
    ../cc3200-sdk/simplelink/include
    ../cc3200-sdk/simplelink/source
    ../cc3200-sdk/third_party/FreeRTOS/source
    ../cc3200-sdk/third_party/FreeRTOS/source/include
    ../cc3200-sdk/third_party/FreeRTOS/source/portable/GCC/ARM_CM4
    ../cc3200-sdk/middleware/framework/pm
    ../cc3200-sdk/middleware/framework/timer
    ../cc3200-sdk/middleware/driver/
    ../cc3200-sdk/middleware/driver/hal
    ../cc3200-sdk/middleware/soc
    ../cc3200-sdk/middleware
    ../cc3200-sdk/simplelink_extlib/include
    ../cc3200-sdk/simplelink_extlib/flc
    ../cc3200-sdk/simplelink_extlib/ota
    ../common
    include
)

FILE(GLOB sources
    ../cc3200-sdk/driverlib/*.c
    ../cc3200-sdk/oslib/osi_freertos.c
    ../cc3200-sdk/simplelink/cc_pal.c
    ../cc3200-sdk/simplelink/source/*.c
    ../cc3200-sdk/third_party/FreeRTOS/source/*.c
    ../cc3200-sdk/third_party/FreeRTOS/source/portable/GCC/ARM_CM4/*.c
    ../cc3200-sdk/third_party/FreeRTOS/source/portable/MemMang/heap_3.c
    ../cc3200-sdk/middleware/driver/*.c
    ../cc3200-sdk/middleware/driver/harl/*.c
    ../cc3200-sdk/middleware/framework/pm/*.c
    ../cc3200-sdk/middleware/framework/timer/*.c
    ../cc3200-sdk/middleware/soc/*.c
    ../cc3200-sdk/simplelink_extlib/flc/*.c
    ../cc3200-sdk/simplelink_extlib/ota/*.c
    src/*.c
)

ADD_LIBRARY(ti_cc3200_extra ${sources})
