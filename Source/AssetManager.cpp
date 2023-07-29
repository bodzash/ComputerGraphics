#include "AssetManager.h"

void AssetManager::Init()
{
    Shaders.Init();
    Uniforms.Init();
}

void AssetManager::Shutdown()
{
    Shaders.Shutdown();
    Uniforms.Init();
}
