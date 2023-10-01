#include "UniformManager.h"

// KEEP IN SYNC WITH SHADER
#define MAX_BONES 50

void UniformManager::Init()
{
    // Samplers
    Diffuse = bgfx::createUniform("s_Diffuse", bgfx::UniformType::Sampler);
    Specular = bgfx::createUniform("s_Specular", bgfx::UniformType::Sampler);
    Normal = bgfx::createUniform("s_Normal", bgfx::UniformType::Sampler);

    // Regular
    Model = bgfx::createUniform("u_Model", bgfx::UniformType::Mat4);
    InverseModel = bgfx::createUniform("u_InverseModel", bgfx::UniformType::Mat4);
    ProjView = bgfx::createUniform("u_ProjView", bgfx::UniformType::Mat4);

    // Bones
    Bones = bgfx::createUniform("u_Bones", bgfx::UniformType::Mat4, MAX_BONES);

    // ???
    Material = bgfx::createUniform("u_Material", bgfx::UniformType::Vec4, 1);
    ViewPosition = bgfx::createUniform("u_ViewPosition", bgfx::UniformType::Vec4);
    DirLight = bgfx::createUniform("u_DirLight", bgfx::UniformType::Vec4, 4);
}

void UniformManager::Shutdown()
{
    // Samplers
    bgfx::destroy(Diffuse);
    bgfx::destroy(Specular);
    bgfx::destroy(Normal);
    //bgfx::destroy(Emissive);
    //bgfx::destroy(Ambient);

    // Regular
    bgfx::destroy(Model);
    bgfx::destroy(InverseModel);
    bgfx::destroy(ProjView);

    // Bones
    bgfx::destroy(Bones);

    // ???
    bgfx::destroy(Material);
    bgfx::destroy(ViewPosition);
    bgfx::destroy(DirLight);    
}