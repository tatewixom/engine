cmake_minimum_required(VERSION 3.16)

include(nuketargets)

add_executable(
  nuke_enginee
  private/EntryPoint.cpp
)

# setup_target(nuke_engine)

set(
  NUKE_DEPENDENCIES
  nuke_core
)

target_link_libraries(
  nuke_enginee
  PRIVATE
  ${NUKE_DEPENDENCIES}
)