cmake_minimum_required(VERSION 3.16)

include(nuketargets)

set(
  NUKE_SOURCE_FILES 
  private/State.cpp
)

add_library(
  nuke_state
  STATIC
  ${NUKE_SOURCE_FILES}
)

setup_target(nuke_state)

target_include_directories(
  nuke_state
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/public
)