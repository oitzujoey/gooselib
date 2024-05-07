cmake_minimum_required(VERSION 3.10)

project(gooselib VERSION 1.0)

add_library(gooselib STATIC
  gooselib/gooselib_allocator.c
  gooselib/gooselib_string.c
  gooselib/gooselib_array.c)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIRECTORY}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIRECTORY}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIRECTORY}")


add_executable(reverse examples/reverse.c)

target_compile_options(reverse PUBLIC -Wall -Wextra -Wpedantic -Werror -Wdouble-promotion)

target_link_libraries(reverse PUBLIC gooselib asan)
