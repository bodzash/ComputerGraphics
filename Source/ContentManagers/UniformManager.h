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

    // Regular
    bgfx::UniformHandle Model;
    bgfx::UniformHandle ProjView;

    // Bone
    bgfx::UniformHandle Bones;

    void Init();
    void Shutdown();

    static UniformManager& Get()
    {
        static UniformManager s_Instance;
        return s_Instance;
    }

private:
};