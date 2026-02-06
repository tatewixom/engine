cmake_minimum_required(VERSION 3.16)

add_library(
  nuke_time
  STATIC
  private/Phase.cpp
)

target_include_directories(
  nuke_time
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/public
)

target_compile_features(
  nuke_time
  PUBLIC 
  cxx_std_23
)