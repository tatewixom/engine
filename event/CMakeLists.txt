cmake_minimum_required(VERSION 3.16)

include(nuketargets)

set(
  NUKE_SOURCE_FILES 
  private/Event.cpp
  private/KeyEvent.cpp
)

add_library(
  nuke_event
  STATIC
  ${NUKE_SOURCE_FILES}
)

setup_target(nuke_event)

target_include_directories(
  nuke_event
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/public
)