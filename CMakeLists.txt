cmake_minimum_required(VERSION 3.16)

# Define project
project(Shit)

# GLFW flags
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# BGFX flags
set(BGFX_AMALGAMATED OFF CACHE BOOL "" FORCE)
set(BX_AMALGAMATED OFF CACHE BOOL "" FORCE)
set(BGFX_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BGFX_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(BGFX_BUILD_TOOLS_BIN2C OFF CACHE BOOL "" FORCE)
set(BGFX_BUILD_TOOLS_GEOMETRY OFF CACHE BOOL "" FORCE)
set(BGFX_BUILD_TOOLS_SHADER ON CACHE BOOL "" FORCE)
set(BGFX_BUILD_TOOLS_TEXTURE ON CACHE BOOL "" FORCE)
set(BGFX_CONFIG_MULTITHREADED ON CACHE BOOL "" FORCE)
set(BGFX_INSTALL OFF CACHE BOOL "No install nonono" FORCE)

# ASSIMP flags
set(ASSIMP_LIBRARY_SUFFIX "" CACHE STRING "Suffix to append to library names")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)

# Definitions
add_compile_definitions(BX_CONFIG_DEBUG)

# Subdirectories
add_subdirectory(glfw)
add_subdirectory(bgfx.cmake)
add_subdirectory(External/assimp)

# Add exe
add_executable(Shit Main.cpp Metadata.rc)

target_include_directories(Shit PUBLIC glfw/include)
target_include_directories(Shit PUBLIC glm/glm)
target_include_directories(Shit PUBLIC bgfx.cmake/bx/include)
target_include_directories(Shit PUBLIC bgfx.cmake/bimg/include)
target_include_directories(Shit PUBLIC bgfx.cmake/bgfx/include)
target_include_directories(Shit PUBLIC tinyobjloader)
target_include_directories(Shit PUBLIC assimp)

# Link libs
target_link_libraries(Shit PRIVATE glfw bgfx bx assimp)