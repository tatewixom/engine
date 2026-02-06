cmake_minimum_required(VERSION 3.16)

add_library(
  nuke_state
  private/State.cpp
)

target_include_directories(
  nuke_state
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/public
)

target_compile_features(
  nuke_state
  PUBLIC 
  cxx_std_23
)