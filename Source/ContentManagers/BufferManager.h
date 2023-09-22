#pragma once
#include "bgfx/bgfx.h"

class BufferManager
{
public:
    BufferManager() = default;
    BufferManager(const BufferManager&) = delete;
    BufferManager(BufferManager&&) = delete;

    static BufferManager& Get()
    {
        static BufferManager s_Instance;
        return s_Instance;
    }

    // Layouts
    bgfx::VertexLayout SkyboxVL;
    bgfx::VertexLayout StaticMeshVL;
    bgfx::VertexLayout SkinnedMeshVL;

    // Buffers
    bgfx::VertexBufferHandle SkyboxVBO;
    bgfx::IndexBufferHandle SkyboxEBO;

    void Init();
    void Shutdown();
};