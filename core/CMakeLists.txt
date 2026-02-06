cmake_minimum_required(VERSION 3.16)

add_library(
  nuke_core
  private/Callback.cpp
  private/Camera.cpp
  private/File.cpp
  private/Game.cpp
  private/Keyboard.cpp
  private/Mouse.cpp
  private/Window.cpp
)

target_include_directories(
  nuke_core
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/public
)

target_link_libraries(
  nuke_core
  PRIVATE
  glad
  glm
  glfw
  stb
  nuke_time
  nuke_event
  nuke_state
)

target_compile_features(
  nuke_core
  PUBLIC 
  cxx_std_23
)