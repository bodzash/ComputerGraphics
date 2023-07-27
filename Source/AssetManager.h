#pragma once
#include "bgfx/bgfx.h"
#include "ShaderManager.h"

// Needs to be a singleton
class AssetManager
{
public:
    ShaderManager Shaders;
    // UniformManager Uniforms;

    void Init();
    void Shutdown();
private:

};