cmake_minimum_required(VERSION 3.16)

add_library(
  nuke_event
  private/Event.cpp
  private/KeyEvent.cpp
)

target_include_directories(
  nuke_event
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/public
)

target_compile_features(
  nuke_event
  PUBLIC 
  cxx_std_23
)