cmake_minimum_required(VERSION 3.16)

add_library(
  stb
  STATIC
  private/stb_impl.cpp
)

target_include_directories(
  stb
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/public
)