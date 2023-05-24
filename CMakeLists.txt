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
set(BGFX_BUILD_EXAMPLES ON CACHE BOOL "" FORCE)
set(BGFX_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(BGFX_BUILD_TOOLS_BIN2C OFF CACHE BOOL "" FORCE)
set(BGFX_BUILD_TOOLS_GEOMETRY OFF CACHE BOOL "" FORCE)
set(BGFX_BUILD_TOOLS_SHADER OFF CACHE BOOL "" FORCE)
set(BGFX_BUILD_TOOLS_TEXTURE OFF CACHE BOOL "" FORCE)
set(BGFX_CONFIG_MULTITHREADED OFF CACHE BOOL "" FORCE) # TEMPORARY
set(BGFX_INSTALL OFF CACHE BOOL "" FORCE)

# 
add_compile_definitions(BX_CONFIG_DEBUG)


# Subdirectories
add_subdirectory(glfw)
add_subdirectory(bgfx.cmake)

# Add exe
add_executable(Shit Main.cpp stb_image.cpp)

target_include_directories(Shit PUBLIC glfw/include)
target_include_directories(Shit PUBLIC glm/glm)
target_include_directories(Shit PUBLIC bgfx.cmake/bx/include)
target_include_directories(Shit PUBLIC bgfx.cmake/bimg/include)
target_include_directories(Shit PUBLIC bgfx.cmake/bgfx/include)

# Link libs
target_link_libraries(Shit PRIVATE glfw bgfx bx)