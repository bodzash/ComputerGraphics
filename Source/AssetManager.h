#pragma once
#include "bgfx/bgfx.h"
#include "ShaderManager.h"
#include "UniformManager.h"

// Needs to be a singleton
// Should be renamed to ResourceManager
class AssetManager
{
public:
    ShaderManager Shaders;
    UniformManager Uniforms;

    void Init();
    void Shutdown();
private:

};