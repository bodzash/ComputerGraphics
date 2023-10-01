#pragma once
#include <string>
#include <bgfx/bgfx.h>
#include "../Utility.h"

// We are just gonna store shader program "assets" as members because we are goinng to use the renderer
// as some pre-defined thing, REMEMBER: this isn't a generalist renderer / engine!!!
// or we can use an <unordered_map> idgaf
class ShaderManager final
{
public:
    ShaderManager() = default;
    ShaderManager(const ShaderManager&) = delete;
    ShaderManager(ShaderManager&&) = delete;

    static ShaderManager& Get()
    {
        static ShaderManager s_Instance;
        return s_Instance;
    }

    // SHOULD BE A MAP
    // std::unordered_map<std::string, bgfx::ProgramHandle> Shaders;
    bgfx::ProgramHandle StaticMesh;
    bgfx::ProgramHandle SkinnedMesh;
    bgfx::ProgramHandle ScreenQuad;
    bgfx::ProgramHandle TransQuad;
    bgfx::ProgramHandle Skybox;
    bgfx::ProgramHandle GBuffer;
    bgfx::ProgramHandle LightingPass;
    // More to come...

    //inline static ShaderManager& Get() { return s_Instance; }
    void Init();
    void Shutdown();

private:
    //static ShaderManager s_Instance;

    bgfx::ShaderHandle LoadShader(const std::string& fileName);
    bgfx::ProgramHandle LoadShaderProgram(const std::string& fileName);
};