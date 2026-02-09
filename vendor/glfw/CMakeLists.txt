cmake_minimum_required(VERSION 3.16)

include(nuketargets)

file(GLOB GLFW_C_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/private/*.c")
file(GLOB GLFW_H_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/private/*.h")
# file(GLOB M_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/private/*.m")
# file(GLOB C_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/private/*.in")

set(GLFW_SOURCE_FILES ${GLFW_C_SOURCE_FILES} ${GLFW_H_SOURCE_FILES})

add_library(
  glfw
  STATIC
  ${GLFW_SOURCE_FILES}
)

setup_target(glfw)

target_include_directories(
  glfw
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/public
)