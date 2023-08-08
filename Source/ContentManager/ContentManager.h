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

    ShaderManager Shaders;
    UniformManager Uniforms;
    TextureManager Textures;
    ModelManager Models;

    // Should be rewritten to 
    // ContentManager::GetUniform().Diffuse;
    // ContentManager::GetShader().Skybox;
    // ContentManager::GetTexture()["Textures/SkyboxDay.dds"];
    // ContentManager::GetModel()["Models/Jack/Jack.dae"];
    // unsure about the bellow 2
    // ContentManager::GetAnimation()
    // ContentManager::GetSkeleton()

    // This will be not needed MAYBE?
    static ContentManager& Get()
    {
        static ContentManager s_Instance;
        return s_Instance;
    }

    void Init();
    void Shutdown();
private:
};