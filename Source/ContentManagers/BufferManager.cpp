#include "BufferManager.h"

void BufferManager::Init()
{
#pragma region StaticMesh

    StaticMeshVL.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .end();

#pragma endregion

#pragma region SkinnedMeshVL

    SkinnedMeshVL.begin()
        // NORMALS ARE NOT WEVEN NEEDED LOOOOL
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Indices, 4, bgfx::AttribType::Float) // BLENDINDICES
        .add(bgfx::Attrib::Weight, 4, bgfx::AttribType::Float) // BLENDWEIGHT
        .end();

#pragma endregion

}

void BufferManager::Shutdown() {}
