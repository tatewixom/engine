cmake_minimum_required(VERSION 3.16)

set(
  DOCTEST_SOURCE_FILES
  private/doctest_impl.cpp
)

add_library(
  doctest
  STATIC
  ${DOCTEST_SOURCE_FILES}
)

target_include_directories(
  doctest
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/public
)