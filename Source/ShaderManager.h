#pragma once
#include <string>
#include "Utility.h"
#include "bgfx/bgfx.h"

// We are just gonna store shader program "assets" as members because we are goinng to use the renderer
// as some pre-defined thing, REMEMBER: this isn't a generalist renderer / engine!!!
// or we can use an <unordered_map> idgaf
// This needs to be a singleton
class ShaderManager
{
public:
    // std::unordered_map<std::string, bgfx::ProgramHandle> Shaders;
    bgfx::ProgramHandle StaticMesh;
    bgfx::ProgramHandle ScreenQuad;
    bgfx::ProgramHandle TransQuad;
    bgfx::ProgramHandle Skybox;
    // More to come...

    //inline static ShaderManager& Get() { return s_Instance; }
    void Init();
    void Shutdown();

private:
    //static ShaderManager s_Instance;

    void LoadAllShaders();
    bgfx::ShaderHandle LoadShader(const std::string& fileName);
    bgfx::ProgramHandle LoadShaderProgram(const std::string& fileName);
};