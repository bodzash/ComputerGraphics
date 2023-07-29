#include "ShaderManager.h"

void ShaderManager::Init()
{
    StaticMesh = LoadShaderProgram("StaticMesh");
    ScreenQuad = LoadShaderProgram("ScreenQuad");
    TransQuad = LoadShaderProgram("TransQuad");
    Skybox = LoadShaderProgram("Skybox");
    GBuffer = LoadShaderProgram("GBuffer");
    LightingPass = LoadShaderProgram("LightingPass");
}

void ShaderManager::Shutdown()
{
    bgfx::destroy(StaticMesh);
    bgfx::destroy(ScreenQuad);
    bgfx::destroy(TransQuad);
    bgfx::destroy(Skybox);
    bgfx::destroy(GBuffer);
    bgfx::destroy(LightingPass);

}

bgfx::ShaderHandle ShaderManager::LoadShader(const std::string& fileName)
{
    std::string filePath = "";

    switch(bgfx::getRendererType())
    {
        case bgfx::RendererType::Noop:
        case bgfx::RendererType::Direct3D9:  filePath = "Resources/Shaders/DirectX9/";   break;
        case bgfx::RendererType::Direct3D11:
        case bgfx::RendererType::Direct3D12: filePath = "Resources/Shaders/DirectX11/";  break;
    }

    return bgfx::createShader(Utility::LoadBinaryData(filePath + fileName));
}

bgfx::ProgramHandle ShaderManager::LoadShaderProgram(const std::string& fileName)
{
    bgfx::ShaderHandle vsh = LoadShader(fileName + ".bvs");
    bgfx::ShaderHandle fsh = LoadShader(fileName + ".bfs");

    return bgfx::createProgram(vsh, fsh, true);
}