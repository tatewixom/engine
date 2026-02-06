cmake_minimum_required(VERSION 3.16)

add_library(
  glad
  private/glad.c
)

target_include_directories(
  glad
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/public
)

target_compile_features(
  glad
  PUBLIC 
  c_std_23
)