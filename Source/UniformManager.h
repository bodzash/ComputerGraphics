#pragma once
#include "bgfx/bgfx.h"

class UniformManager
{
public:
    // Samplers
    bgfx::UniformHandle Diffuse;
    bgfx::UniformHandle Specular;

    // Regular
    bgfx::UniformHandle Model;
    bgfx::UniformHandle InverseModel;
    bgfx::UniformHandle ProjView;

    void Init();
    void Shutdown();

private:
};