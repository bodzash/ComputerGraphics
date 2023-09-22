#include "Shader.h"

Shader::Shader(const std::string& fileName)
{
    Handle = LoadShaderProgram(fileName);
}

void Shader::SetUniform(const std::string& name, const void* data, uint16_t num)
{
    bgfx::setUniform(Uniforms[name], data, num);
}

void Shader::CreateUniform(const std::string& name, bgfx::UniformType::Enum type, uint16_t num)
{
    Uniforms[name] = bgfx::createUniform(name.c_str(), type, num);
}

void Shader::DestroyUniforms()
{
    for (std::pair<std::string, bgfx::UniformHandle> uniform : Uniforms)
    {
        bgfx::destroy(uniform.second);
    }
}

void Shader::Destroy()
{
    DestroyUniforms();
    bgfx::destroy(Handle);
}

bgfx::ShaderHandle Shader::LoadShader(const std::string &fileName)
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

bgfx::ProgramHandle Shader::LoadShaderProgram(const std::string& fileName)
{
    bgfx::ShaderHandle vsh = LoadShader(fileName + ".bvs");
    bgfx::ShaderHandle fsh = LoadShader(fileName + ".bfs");

    return bgfx::createProgram(vsh, fsh, true);
}