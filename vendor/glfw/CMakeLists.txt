cmake_minimum_required(VERSION 3.16)

file(GLOB C_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/private/*.c")
file(GLOB H_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/private/*.h")
# file(GLOB M_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/private/*.m")
# file(GLOB C_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/private/*.in")

set(SOURCE_FILES ${C_SOURCE_FILES} ${H_SOURCE_FILES})

add_library(
  glfw
  STATIC
  ${SOURCE_FILES}
)

target_include_directories(
  glfw
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/public
)

target_compile_features(
  glfw
  PUBLIC
  cxx_std_23
)