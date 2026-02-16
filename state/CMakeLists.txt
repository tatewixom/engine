cmake_minimum_required(VERSION 3.16)

include(nuketargets)

set(
  NUKE_SOURCE_FILES 
  private/Ground.cpp
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

set(
  NUKE_DEPENDENCIES
  random
  glm
  nuke_core
  nuke_model
  nuke_graphics
)

target_link_libraries(
  nuke_state
  PUBLIC
  ${NUKE_DEPENDENCIES}
)