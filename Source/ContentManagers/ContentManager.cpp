#include "ContentManager.h"

void ContentManager::Init()
{
    Shaders.Init();
    Uniforms.Init();
}

void ContentManager::Shutdown()
{
    Shaders.Shutdown();
    Uniforms.Init();
}