cmake_minimum_required(VERSION 3.16)

set(
  GLAD_SOURCE_FILES
  private/glad.c
)

add_library(
  glad
  STATIC
  ${GLAD_SOURCE_FILES}
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