cmake_minimum_required(VERSION 3.16)

# doctest
enable_testing()
add_subdirectory(doctest)

# glad
add_subdirectory(glad)

# glfw
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)

# glm
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
set(GLM_ENABLE_CXX_20 ON CACHE BOOL "" FORCE)
add_subdirectory(glm)

# random (learncpp)
add_subdirectory(random)

# stb
add_subdirectory(stb)

# tinygltf
add_subdirectory(tinygltf)

