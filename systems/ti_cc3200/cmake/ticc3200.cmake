# Add precompiled args
add_definitions(-Dgcc)
add_definitions(-DSL_PLATFORM_MULTI_THREADED)
add_definitions(-DUSE_FREERTOS)
add_definitions(-DxTARGET_IS_CC3200)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)

set(CMAKE_AR arm-none-eabi-ar CACHE STRING "arm-none-eabi-ar" FORCE)
set(CMAKE_LD arm-none-eabi-ld CACHE STRING "arm-none-eabi-ld" FORCE)
set(OBJCOPY arm-none-eabi-objcopy CACHE STRING "arm-none-eabi-objcopy" FORCE)
set(OBJDUMP arm-none-eabi-objdump CACHE STRING "arm-none-eabi-objdump" FORCE)

# Set compiler arguments
SET(CMAKE_C_FLAGS "-mcpu=cortex-m4 -mthumb -ffunction-sections -fdata-sections -MD -std=c11 -O0 -g --specs=nosys.specs")

#set(CMAKE_C_LINK_EXECUTABLE
#    "<CMAKE_C_COMPILER> -Wl,--entry=ResetISR,--gc-sections -o <TARGET> <OBJECTS> <LINK_LIBRARIES>")


set(CMAKE_EXE_LINKER_FLAGS "-T systems/ti_cc3200/linker.ld -Xlinker --gc-sections")