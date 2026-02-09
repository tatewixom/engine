cmake_minimum_required(VERSION 3.16)

include(nuketargets)

set(
  STB_SOURCE_FILES
  private/stb_impl.cpp
)

add_library(
  stb
  STATIC
  ${STB_SOURCE_FILES}
)

setup_target(stb)

target_include_directories(
  stb
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/public
)