cmake_minimum_required(VERSION 3.16)

# --
# this file is meant for functions that are target helpers
# --

# function for setting up each target with correct settings
function(setup_target target_name)
  target_compile_features(
    ${target_name}
    PUBLIC
    cxx_std_23
  )

  get_target_property(
    COMPILER_STANDARD
    ${target_name}
    CXX_STANDARD
  )

  message(VERBOSE "Setting up target: ${target_name}")
  message(VERBOSE "Using compiler standard: C++${COMPILER_STANDARD}")
endfunction()