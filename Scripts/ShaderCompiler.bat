@ECHO OFF
ECHO [INFO]: Compiling shaders...

REM Static mesh
@ECHO OFF
START ../Binaries/shaderc -f ../Content/Shaders/StaticMesh/StaticMesh.vert^
 -o ../Content/Shaders/DirectX9/StaticMesh.bvs^
 --type vertex -i ../External/bgfx.cmake/bgfx/src --platform windows^
 --varyingdef ../Content/Shaders/StaticMesh/StaticMesh.def -p s_3_0
@ECHO ON

@ECHO OFF
START ../Binaries/shaderc -f ../Content/Shaders/StaticMesh/StaticMesh.frag^
 -o ../Content/Shaders/DirectX9/StaticMesh.bfs^
 --type fragment -i ../External/bgfx.cmake/bgfx/src --platform windows^
 --varyingdef ../Content/Shaders/StaticMesh/StaticMesh.def -p s_3_0
@ECHO ON

@ECHO OFF
REM Skybox

@ECHO OFF
START ../Binaries/shaderc -f ../Content/Shaders/Skybox/Skybox.vert^
 -o ../Content/Shaders/DirectX9/Skybox.bvs^
 --type vertex -i ../External/bgfx.cmake/bgfx/src --platform windows^
 --varyingdef ../Content/Shaders/Skybox/Skybox.def -p s_3_0
@ECHO ON

@ECHO OFF
START ../Binaries/shaderc -f ../Content/Shaders/Skybox/Skybox.frag^
 -o ../Content/Shaders/DirectX9/Skybox.bfs^
 --type fragment -i ../External/bgfx.cmake/bgfx/src --platform windows^
 --varyingdef ../Content/Shaders/Skybox/Skybox.def -p s_3_0
@ECHO ON

@ECHO OFF
REM ScreenQuad

@ECHO OFF
START ../Binaries/shaderc -f ../Content/Shaders/ScreenQuad/ScreenQuad.vert^
 -o ../Content/Shaders/DirectX9/ScreenQuad.bvs^
 --type vertex -i ../External/bgfx.cmake/bgfx/src --platform windows^
 --varyingdef ../Content/Shaders/ScreenQuad/ScreenQuad.def -p s_3_0
@ECHO ON

@ECHO OFF
START ../Binaries/shaderc -f ../Content/Shaders/ScreenQuad/ScreenQuad.frag^
 -o ../Content/Shaders/DirectX9/ScreenQuad.bfs^
 --type fragment -i ../External/bgfx.cmake/bgfx/src --platform windows^
 --varyingdef ../Content/Shaders/ScreenQuad/ScreenQuad.def -p s_3_0
@ECHO ON

@ECHO OFF
REM TransQuad

@ECHO OFF
START ../Binaries/shaderc -f ../Content/Shaders/TransQuad/TransQuad.vert^
 -o ../Content/Shaders/DirectX9/TransQuad.bvs^
 --type vertex -i ../External/bgfx.cmake/bgfx/src --platform windows^
 --varyingdef ../Content/Shaders/TransQuad/TransQuad.def -p s_3_0
@ECHO ON

@ECHO OFF
START ../Binaries/shaderc -f ../Content/Shaders/TransQuad/TransQuad.frag^
 -o ../Content/Shaders/DirectX9/TransQuad.bfs^
 --type fragment -i ../External/bgfx.cmake/bgfx/src --platform windows^
 --varyingdef ../Content/Shaders/TransQuad/TransQuad.def -p s_3_0
@ECHO ON

@ECHO OFF
REM GBuffer

@ECHO OFF
START ../Binaries/shaderc -f ../Content/Shaders/Deferred/GBuffer.vert^
 -o ../Content/Shaders/DirectX9/GBuffer.bvs^
 --type vertex -i ../External/bgfx.cmake/bgfx/src --platform windows^
 --varyingdef ../Content/Shaders/Deferred/GBuffer.def -p s_3_0
@ECHO ON

@ECHO OFF
START ../Binaries/shaderc -f ../Content/Shaders/Deferred/GBuffer.frag^
 -o ../Content/Shaders/DirectX9/GBuffer.bfs^
 --type fragment -i ../External/bgfx.cmake/bgfx/src --platform windows^
 --varyingdef ../Content/Shaders/Deferred/GBuffer.def -p s_3_0
@ECHO ON

@ECHO OFF
REM LightingPass

@ECHO OFF
START ../Binaries/shaderc -f ../Content/Shaders/Deferred/LightingPass.vert^
 -o ../Content/Shaders/DirectX9/LightingPass.bvs^
 --type vertex -i ../External/bgfx.cmake/bgfx/src --platform windows^
 --varyingdef ../Content/Shaders/Deferred/LightingPass.def -p s_3_0
@ECHO ON

@ECHO OFF
START ../Binaries/shaderc -f ../Content/Shaders/Deferred/LightingPass.frag^
 -o ../Content/Shaders/DirectX9/LightingPass.bfs^
 --type fragment -i ../External/bgfx.cmake/bgfx/src --platform windows^
 --varyingdef ../Content/Shaders/Deferred/LightingPass.def -p s_3_0
@ECHO ON

@ECHO OFF
REM shaderc std out will be displayed here
PAUSE