cmake_minimum_required(VERSION 3.16)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

project(
  nuke_engine 
  VERSION "1.0.0"
  HOMEPAGE_URL "https://github.com/tatewixom/engine"
  DESCRIPTION "Light-weight 3D game engine"
  LANGUAGES CXX C
)

# allows for vscode to make a "compile_commands.json" file for 
# correct intellisense integration
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# making sure to enforce c++23 
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# I don't like the lib prefixes or suffixes
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")

add_subdirectory(vendor)

add_subdirectory(event)
add_subdirectory(graphics)
add_subdirectory(model)
add_subdirectory(state)
add_subdirectory(time)
add_subdirectory(core)