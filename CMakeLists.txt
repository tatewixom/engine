cmake_minimum_required(VERSION 3.16)
project(engine VERSION "1.0.0")

set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")

add_subdirectory(vendor)

add_subdirectory(core)
add_subdirectory(event)
add_subdirectory(state)
add_subdirectory(time)