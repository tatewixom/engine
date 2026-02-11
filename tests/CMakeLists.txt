cmake_minimum_required(VERSION 3.16)

set(
  NUKE_UNIT_TESTS_SOURCE
  private/test_tests.cpp
)

add_executable(
  nuke_unit_tests
  ${NUKE_UNIT_TESTS_SOURCE}
)

setup_target(nuke_unit_tests)

target_link_libraries(
  nuke_unit_tests
  PUBLIC
  doctest
)

add_test(NAME nuke_unit_tests COMMAND nuke_unit_tests)