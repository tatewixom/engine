cmake_minimum_required(VERSION 3.16)

include(nuketargets)

set(
  NUKE_SOURCE_FILES
  private/Buffer.cpp
  private/Shader.cpp
)

add_library(
  nuke_graphics
  STATIC
  ${NUKE_SOURCE_FILES}
)

setup_target(nuke_graphics)

target_include_directories(
  nuke_graphics
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/public
)

set(
  NUKE_DEPENDENCIES
  glad 
  glm
  nuke_core
)

target_link_libraries(
  nuke_graphics
  PUBLIC
  ${NUKE_DEPENDENCIES}
)