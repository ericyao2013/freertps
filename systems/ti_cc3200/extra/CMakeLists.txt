add_definitions(-Dgcc)

include(${PROJECT_SOURCE_DIR}/systems/${SYSTEM}/extra/cmake/ticc3200_extra.cmake)

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
    src/*.c
)

ADD_LIBRARY(ti_cc3200_extra ${sources})
