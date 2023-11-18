cmake_minimum_required(VERSION 3.16)

# Definitions
# add_compile_definitions(AX_PLATFORM_WINDOWS)
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
set(BGFX_AMALGAMATED ON CACHE BOOL "No amalgamated" FORCE)
set(BX_AMALGAMATED ON CACHE BOOL "No amalgamated" FORCE)
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
set(ASSIMP_NO_EXPORT ON CACHE BOOL "No exporters" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "No all importers" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "Enable FBX importer" FORCE)
set(ASSIMP_BUILD_COLLADA_IMPORTER ON CACHE BOOL "Enable COLLADA importer" FORCE)

# TinyGLTF flags
# set(TINYGLTF_HEADER_ONLY ON CACHE BOOL "" FORCE)
set(TINYGLTF_INSTALL OFF CACHE BOOL "" FORCE)
set(TINYGLTF_NO_EXTERNAL_IMAGE ON CACHE BOOL "" FORCE)

# OZZ flags
#set(ozz_build_tools OFF CACHE BOOL "No tools" FORCE)
#set(ozz_build_fbx OFF CACHE BOOL "No FBX" FORCE)
#set(ozz_build_gltf OFF CACHE BOOL "No GLTF" FORCE)
#set(ozz_build_samples OFF CACHE BOOL "No samples" FORCE)
#set(ozz_build_howtos OFF CACHE BOOL "No howto" FORCE)
#set(ozz_build_tests OFF CACHE BOOL "No tests" FORCE)


# Third party definitions
# add_compile_definitions(BX_CONFIG_DEBUG)

# Subdirectories (folders that have a CMakeLists.txt file)
add_subdirectory(External/glfw)
add_subdirectory(External/bgfx.cmake)
add_subdirectory(External/assimp)
#add_subdirectory(External/tinygltf)

# Executable
add_executable(
    Shit
    #Content/Metadata/Metadata.rc
    
    Source/Main.cpp
    # Rendering
    Source/Texture.cpp
    #Source/Shader.cpp
    Source/Model.cpp
    Source/Bone.cpp
    Source/Animation.cpp
    Source/Animator.cpp
    Source/StaticModel.cpp
    #Source/SkinnedModel.cpp
    # Asset Managers
    Source/ContentManagers/ContentManager.cpp
    Source/ContentManagers/ShaderManager.cpp
    Source/ContentManagers/UniformManager.cpp
    Source/ContentManagers/ModelManager.cpp
    Source/ContentManagers/TextureManager.cpp
    Source/ContentManagers/BufferManager.cpp
    # Misc
    Source/Utility.cpp
)

# Properties
# On windows this removes the console and only shows the GLFW window
# set_target_properties(Shit PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS")
# On GCC/Linux we can add the -mwindow flag to remove the console as well
# But game client will be windows only

# Include libraries
target_include_directories(Shit PUBLIC External/glfw/include)
target_include_directories(Shit PUBLIC External/SDL2/include)
target_include_directories(Shit PUBLIC External/glm/glm)
target_include_directories(Shit PUBLIC External/bgfx.cmake/bx/include)
target_include_directories(Shit PUBLIC External/bgfx.cmake/bimg/include)
target_include_directories(Shit PUBLIC External/bgfx.cmake/bgfx/include)
target_include_directories(Shit PUBLIC External/assimp)
# target_include_directories(Shit PUBLIC External/tinygltf)

# Link libraries
#target_link_libraries(Shit PRIVATE tinygltf)
target_link_libraries(Shit PRIVATE glfw bgfx bx assimp SDL2 SDL2main)
#target_link_libraries(Shit PRIVATE glfw bgfx bx assimp ozz_animation)