add_definitions(-DUSE_FREERTOS)

include(${PROJECT_SOURCE_DIR}/systems/ti_cc3200/extra/cmake/ticc3200_extra.cmake)

set(CMAKE_EXECUTABLE_SUFFIX .elf)
set(CMAKE_EXE_LINKER_FLAGS "-T ${PROJECT_SOURCE_DIR}/systems/ti_cc3200/linker.ld -Xlinker --gc-sections")

freertps_add_executable(talker-listener_cc3200 talker-listener_cc3200.c)
