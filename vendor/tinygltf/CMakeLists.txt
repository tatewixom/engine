cmake_minimum_required(VERSION 3.16)

add_library(
  tinygltf
  STATIC
  private/tiny_gltf.cc
)

target_include_directories(
  tinygltf
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/public
)

target_link_libraries(
  tinygltf
  PUBLIC
  stb
)

target_compile_features(
  tinygltf
  PUBLIC
  cxx_std_23
)