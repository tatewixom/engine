cmake_minimum_required(VERSION 3.16)

include(nuketargets)

# source
set(
  TINYGLTF_SOURCE_FILES
  private/tiny_gltf.cc
)

add_library(
  tinygltf
  STATIC
  ${TINYGLTF_SOURCE_FILES}
)

setup_target(tinygltf)

target_include_directories(
  tinygltf
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/public
)

# dependencies
set(
  TINYGLTF_DEPENDENCIES
  stb
)

target_link_libraries(
  tinygltf
  PUBLIC
  ${TINYGLTF_DEPENDENCIES}
)