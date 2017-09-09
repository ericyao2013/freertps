# Add precompiled args
add_definitions(-Dgcc)
add_definitions(-DSL_PLATFORM_MULTI_THREADED)
add_definitions(-DUSE_FREERTOS)
add_definitions(-DUSE_CC3200)
add_definitions(-DxTARGET_IS_CC3200)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)

# Set compiler arguments
set(CMAKE_C_FLAGS "-mcpu=cortex-m4 -mthumb -ffunction-sections -fdata-sections -MD -std=c11 -O0 -g --specs=nosys.specs")

set(CMAKE_EXE_LINKER_FLAGS "-T systems/ti_cc3200/linker.ld -Xlinker --gc-sections")

function(make_bin exe elf bin)
  set(CMAKE_AR arm-none-eabi-ar CACHE STRING "arm-none-eabi-ar" FORCE)
  set(CMAKE_LD arm-none-eabi-ld CACHE STRING "arm-none-eabi-ld" FORCE)
  set(OBJCOPY arm-none-eabi-objcopy CACHE STRING "arm-none-eabi-objcopy" FORCE)
  set(OBJDUMP arm-none-eabi-objdump CACHE STRING "arm-none-eabi-objdump" FORCE)

  add_custom_command(OUTPUT ${bin}
                     COMMAND arm-none-eabi-objcopy -O binary ${elf} ${bin}
                     DEPENDS ${elf}
                     COMMENT "creating ${bin}")
  add_custom_target(${exe}_bin ALL DEPENDS ${bin})

  add_custom_command(OUTPUT ${elf}.objdump
                     COMMAND arm-none-eabi-objdump -S -d ${elf} > ${elf}.objdump
                     DEPENDS ${elf}
                     COMMENT "disassembling ${elf}")
  add_custom_target(${exe}_objdump ALL DEPENDS ${elf}.objdump)
endfunction()
