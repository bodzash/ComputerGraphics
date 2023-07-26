@ECHO OFF
ECHO [INFO]: Compiling shaders...

@ECHO OFF
shaderc -f BasicUniversal.vert -o build/Debug/Shaders/DirectX9/BasicUniversal.bvs --type vertex -i bgfx.cmake/bgfx/src --platform windows --varyingdef BasicUniversal.def -p s_3_0
@ECHO ON

@ECHO OFF
shaderc -f BasicUniversal.frag -o build/Debug/Shaders/DirectX9/BasicUniversal.bfs --type fragment -i bgfx.cmake/bgfx/src --platform windows --varyingdef BasicUniversal.def -p s_3_0
@ECHO ON

@ECHO OFF
shaderc -f Quad.vert -o build/Debug/Shaders/DirectX9/Quad.bvs --type vertex -i bgfx.cmake/bgfx/src --platform windows --varyingdef Quad.def -p s_3_0
@ECHO ON

@ECHO OFF
shaderc -f Quad.frag -o build/Debug/Shaders/DirectX9/Quad.bfs --type fragment -i bgfx.cmake/bgfx/src --platform windows --varyingdef Quad.def -p s_3_0
@ECHO ON

@ECHO OFF
shaderc -f FrameQuad.vert -o build/Debug/Shaders/DirectX9/FrameQuad.bvs --type vertex -i bgfx.cmake/bgfx/src --platform windows --varyingdef FrameQuad.def -p s_3_0
@ECHO ON

@ECHO OFF
shaderc -f FrameQuad.frag -o build/Debug/Shaders/DirectX9/FrameQuad.bfs --type fragment -i bgfx.cmake/bgfx/src --platform windows --varyingdef FrameQuad.def -p s_3_0
@ECHO ON

@ECHO OFF
shaderc -f Skybox.vert -o build/Debug/Shaders/DirectX9/Skybox.bvs --type vertex -i bgfx.cmake/bgfx/src --platform windows --varyingdef Skybox.def -p s_3_0
@ECHO ON

@ECHO OFF
shaderc -f Skybox.frag -o build/Debug/Shaders/DirectX9/Skybox.bfs --type fragment -i bgfx.cmake/bgfx/src --platform windows --varyingdef Skybox.def -p s_3_0
@ECHO ON


@ECHO OFF
REM shaderc std out will be displayed here
PAUSE