@ECHO OFF
ECHO [INFO]: Compiling shaders...

@ECHO OFF
shaderc -f ../Resources/Shaders/StaticMesh/StaticMesh.vert -o ../Resources/Shaders/Directx9/StaticMesh.bvs --type vertex -i ../External/bgfx.cmake/bgfx/src --platform windows --varyingdef ../Resources/Shaders/StaticMesh/StaticMesh.def -p s_3_0
@ECHO ON

@ECHO OFF
shaderc -f ../Resources/Shaders/StaticMesh/StaticMesh.frag -o ../Resources/Shaders/Directx9/StaticMesh.bfs --type fragment -i ../External/bgfx.cmake/bgfx/src --platform windows --varyingdef ../Resources/Shaders/StaticMesh/StaticMesh.def -p s_3_0
@ECHO ON



@ECHO OFF
REM shaderc std out will be displayed here
PAUSE