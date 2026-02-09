cmake_minimum_required(VERSION 3.16)

include(nuketargets)

set(
  NUKE_SOURCE_FILES 
  private/Phase.cpp
)

add_library(
  nuke_time
  STATIC
  ${NUKE_SOURCE_FILES}
)

setup_target(nuke_time)

target_include_directories(
  nuke_time
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/public
)