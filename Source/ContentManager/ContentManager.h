#pragma once
#include <iostream>
#include "bgfx/bgfx.h"
#include "ShaderManager.h"
#include "UniformManager.h"
#include "ModelManager.h"
#include "TextureManager.h"

class ContentManager
{
public:
    ContentManager() = default;
    ContentManager(const ContentManager&) = delete;
    ContentManager(ContentManager&&) = delete;
    // Destructor for a singleton???
    //~ContentManager() { Shutdown(); }

    ShaderManager Shaders;
    UniformManager Uniforms;
    TextureManager Textures;
    ModelManager Models;

    static ContentManager& Get()
    {
        static ContentManager s_Instance;
        return s_Instance;
    }

    void Init();
    void Shutdown();
private:
};