#pragma once
#include <iostream>
#include "bgfx/bgfx.h"
#include "ShaderManager.h"
#include "UniformManager.h"

class ContentManager
{
public:
    ContentManager() = default;
    ContentManager(const ContentManager&) = delete;
    ContentManager(ContentManager&& other) = delete;
    // Destructor for a singleton???
    //~ContentManager() { Shutdown(); }

    ShaderManager Shaders;
    UniformManager Uniforms;

    static ContentManager& Get()
    {
        static ContentManager s_Instance;
        return s_Instance;
    }

    void Init();
    void Shutdown();
private:
};