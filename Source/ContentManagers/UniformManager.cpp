#include "UniformManager.h"

// KEEP IN SYNC WITH SHADER
#define MAX_BONES 50

void UniformManager::Init()
{
    // Samplers
    Diffuse = bgfx::createUniform("s_Diffuse", bgfx::UniformType::Sampler);

    // Regular
    Model = bgfx::createUniform("u_Model", bgfx::UniformType::Mat4);
    ProjView = bgfx::createUniform("u_ProjView", bgfx::UniformType::Mat4); 

    // Bones
    Bones = bgfx::createUniform("u_Bones", bgfx::UniformType::Mat4, MAX_BONES);
}

void UniformManager::Shutdown()
{
    // Samplers
    bgfx::destroy(Diffuse);

    // Regular
    bgfx::destroy(Model);
    bgfx::destroy(ProjView);

    // Bones
    bgfx::destroy(Bones);
}