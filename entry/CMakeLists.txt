cmake_minimum_required(VERSION 3.16)

include(nuketargets)

add_executable(
  nuke_engine
  private/EntryPoint.cpp
)

# setup_target(nuke_engine)

find_package(OpenGL REQUIRED)

set(
  NUKE_DEPENDENCIES
  nuke_graphics
)

target_link_libraries(
  nuke_engine
  PRIVATE
  ${NUKE_DEPENDENCIES}
)

get_target_property(
    COMPILER_STANDARD
    nuke_engine
    CXX_STANDARD
)

message(VERBOSE "Setting up target: nuke_enginee")
message(VERBOSE "Using compiler standard: C++${COMPILER_STANDARD}")