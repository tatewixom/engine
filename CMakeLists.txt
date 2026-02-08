cmake_minimum_required(VERSION 3.16)

project(
  nuke_engine 
  VERSION "1.0.0"
  HOMEPAGE_URL "https://github.com/tatewixom/engine"
  DESCRIPTION "Light-weight 3D game engine"
  LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")

add_subdirectory(vendor)

add_subdirectory(event)
add_subdirectory(state)
add_subdirectory(time)
add_subdirectory(core)