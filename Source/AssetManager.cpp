#include "AssetManager.h"

void AssetManager::Init()
{
    Shaders.Init();
}

void AssetManager::Shutdown()
{
    Shaders.Shutdown();
}
