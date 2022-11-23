cmake_minimum_required(VERSION 3.13)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS_RELEASE "-Os")

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

PROJECT(pico-opl3-player)

add_subdirectory(src)
