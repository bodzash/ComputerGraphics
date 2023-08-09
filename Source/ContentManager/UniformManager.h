#pragma once
#include "bgfx/bgfx.h"

class UniformManager final
{
public:
    // Samplers
    bgfx::UniformHandle Diffuse;
    bgfx::UniformHandle Specular;
    bgfx::UniformHandle Normal;

    // Regular
    bgfx::UniformHandle Model;
    bgfx::UniformHandle InverseModel;
    bgfx::UniformHandle ProjView;

    // ???
    bgfx::UniformHandle Material;
    bgfx::UniformHandle ViewPosition;
    bgfx::UniformHandle DirLight;

    void Init();
    void Shutdown();

private:
};