#pragma once
#include "bgfx/bgfx.h"

class UniformManager final
{
public:
    UniformManager() = default;
    UniformManager(const UniformManager&) = delete;
    UniformManager(UniformManager&&) = delete;
    // Samplers
    bgfx::UniformHandle Diffuse;
    bgfx::UniformHandle Specular;
    bgfx::UniformHandle Normal;

    // Regular
    bgfx::UniformHandle Model;
    bgfx::UniformHandle InverseModel;
    bgfx::UniformHandle ProjView;

    // Bone
    bgfx::UniformHandle Bones;

    // ???
    bgfx::UniformHandle Material;
    bgfx::UniformHandle ViewPosition;
    bgfx::UniformHandle DirLight;

    void Init();
    void Shutdown();

    static UniformManager& Get()
    {
        static UniformManager s_Instance;
        return s_Instance;
    }

private:
};