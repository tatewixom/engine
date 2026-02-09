cmake_minimum_required(VERSION 3.16)

include(nuketargets)

set(
  NUKE_SOURCE_FILES 
  private/Callback.cpp
  private/Camera.cpp
  private/File.cpp
  private/Game.cpp
  private/Keyboard.cpp
  private/Mouse.cpp
  private/Window.cpp
)

add_library(
  nuke_core
  STATIC
  ${NUKE_SOURCE_FILES}
)

setup_target(nuke_core)

target_include_directories(
  nuke_core
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/public
)

set(
  NUKE_DEPENDENCIES
  glad
  glm
  glfw
  stb
  nuke_time
  nuke_event
  nuke_state
)

target_link_libraries(
  nuke_core
  PRIVATE
  ${NUKE_DEPENDENCIES}
)