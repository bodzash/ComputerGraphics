@ECHO OFF
ECHO [INFO]: Compiling shaders...

@ECHO OFF
shaderc -f Basic.vert -o build/Debug/Shaders/DirectX9/Basic.vert.bin --type vertex -i bgfx.cmake/bgfx/src --platform windows --varyingdef Basic.def -p s_3_0
@ECHO ON

@ECHO OFF
shaderc -f Basic.frag -o build/Debug/Shaders/DirectX9/Basic.frag.bin --type fragment -i bgfx.cmake/bgfx/src --platform windows --varyingdef Basic.def -p s_3_0
@ECHO ON

@ECHO OFF
REM shaderc std out will be displayed here
PAUSE