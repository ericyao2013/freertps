set(PROJECT_TEST_NAME ${PROJECT_NAME}_test)

find_package(ament_cmake_gtest REQUIRED)

set(extra_test_libraries)
set(extra_test_env)
set(extra_lib_dirs "${rcl_lib_dir}")

ament_add_gtest(${PROJECT_NAME}_test
    freertps/part_test.cpp
    APPEND_LIBRARY_DIRS )


enable_testing()
