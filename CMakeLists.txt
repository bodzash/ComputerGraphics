cmake_minimum_required(VERSION 3.16)

# Definitions
add_compile_definitions(AX_PLATFORM_WINDOWS)
# add_compile_definitions(AX_BUILD_DISTRIBUTION)

# Project
project(Shit)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# GLFW flags
set(GLFW_BUILD_DOCS OFF CACHE BOOL "No docs" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "No tests" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "No examples" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "No install" FORCE)

# BGFX flags
set(BGFX_AMALGAMATED OFF CACHE BOOL "No amalgamated" FORCE)
set(BX_AMALGAMATED OFF CACHE BOOL "No amalgamated" FORCE)
set(BGFX_BUILD_EXAMPLES OFF CACHE BOOL "No examples" FORCE)
set(BGFX_BUILD_TOOLS OFF CACHE BOOL "Tools off by default" FORCE)
set(BGFX_BUILD_TOOLS_BIN2C OFF CACHE BOOL "Shader to header compiler, fuck no baby" FORCE)
set(BGFX_BUILD_TOOLS_GEOMETRY OFF CACHE BOOL "Do not use" FORCE)
set(BGFX_BUILD_TOOLS_SHADER ON CACHE BOOL "Shader compiler" FORCE)
set(BGFX_BUILD_TOOLS_TEXTURE ON CACHE BOOL "Texture compiler" FORCE)
set(BGFX_CONFIG_MULTITHREADED ON CACHE BOOL "Use multithreading, YES SIR" FORCE)
set(BGFX_INSTALL OFF CACHE BOOL "No install" FORCE)

# ASSIMP flags
set(ASSIMP_LIBRARY_SUFFIX "" CACHE STRING "Suffix to append to library names")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "No DLL" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "No install" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "No tests" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "No exporters" FORCE)

# Third party definitions
# add_compile_definitions(BX_CONFIG_DEBUG)

# Subdirectories
add_subdirectory(External/glfw)
add_subdirectory(External/bgfx.cmake)
add_subdirectory(External/assimp)

# Executable
add_executable(
    Shit
    Resources/Metadata/Metadata.rc
    Source/Main.cpp
    Source/AssetManager.cpp
    Source/ShaderManager.cpp
    Source/Utility.cpp
)

# Properties
# On windows this removes the console and only shows the GLFW window
# set_target_properties(Shit PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS")
# On GCC/Linux we can add the -mwindow flag to remove the console as well
# But game client will be windows only

# Include libraries
target_include_directories(Shit PUBLIC External/glfw/include)
target_include_directories(Shit PUBLIC External/glm/glm)
target_include_directories(Shit PUBLIC External/bgfx.cmake/bx/include)
target_include_directories(Shit PUBLIC External/bgfx.cmake/bimg/include)
target_include_directories(Shit PUBLIC External/bgfx.cmake/bgfx/include)
target_include_directories(Shit PUBLIC External/assimp)

# Link libraries
target_link_libraries(Shit PRIVATE glfw bgfx bx assimp)