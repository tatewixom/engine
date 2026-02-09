cmake_minimum_required(VERSION 3.16)

include(nuketargets)

set(
  NUKE_SOURCE_FILES
  private/Collection.cpp
  private/Ditto.cpp
  private/Mesh.cpp
  private/Model.cpp
  private/Object.cpp
  private/Spaces.cpp
  private/Texture.cpp
)

add_library(
  nuke_model
  STATIC
  ${NUKE_SOURCE_FILES}
)

setup_target(nuke_model)

target_include_directories(
  nuke_model
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/public
)

set(
  NUKE_DEPENDENCIES
  glm
  tinygltf
  random
  nuke_graphics
  nuke_core
)

target_link_libraries(
  nuke_model
  PUBLIC
  ${NUKE_DEPENDENCIES}
)
