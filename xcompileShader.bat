@ECHO OFF
ECHO [INFO]: Compiling shaders...

@ECHO OFF
shaderc -f BasicUniversal.vert -o build/Debug/Shaders/DirectX9/BasicUniversal.bvs --type vertex -i bgfx.cmake/bgfx/src --platform windows --varyingdef BasicUniversal.def -p s_3_0
@ECHO ON

@ECHO OFF
shaderc -f BasicUniversal.frag -o build/Debug/Shaders/DirectX9/BasicUniversal.bfs --type fragment -i bgfx.cmake/bgfx/src --platform windows --varyingdef BasicUniversal.def -p s_3_0
@ECHO ON

@ECHO OFF
REM shaderc std out will be displayed here
PAUSE