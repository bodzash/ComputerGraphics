#pragma once
#include <string>
#include <unordered_map>
#include "bgfx/bgfx.h"
#include "Utility.h"

class Shader
{
public:
    bgfx::ProgramHandle Handle;
    std::unordered_map<std::string, bgfx::UniformHandle> Uniforms;

    Shader(const std::string& fileName);

    void SetUniform(const std::string& name, const void* data, uint16_t num = (uint16_t)1U);
    void Destroy();

    void CreateUniform(const std::string& name, bgfx::UniformType::Enum type, uint16_t num = (uint16_t)1U);
private:
    void DestroyUniforms();
    bgfx::ShaderHandle LoadShader(const std::string& fileName);
    bgfx::ProgramHandle LoadShaderProgram(const std::string& fileName);
};