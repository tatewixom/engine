cmake_minimum_required(VERSION 3.16)

include(nuketargets)

set(
  NUKE_SOURCE_FILES
  private/Interface.cpp
)

add_library(
  nuke_interface
  STATIC
  ${NUKE_SOURCE_FILES}
)

setup_target(nuke_interface)

target_include_directories(
  nuke_interface
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/public
)

set(
  NUKE_DEPENDENCIES
  glm
  nuke_state
  nuke_model
  nuke_core
)

target_link_libraries(
  nuke_interface
  PUBLIC
  ${NUKE_DEPENDENCIES}
)
