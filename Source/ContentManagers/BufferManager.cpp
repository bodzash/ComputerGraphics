#include "BufferManager.h"

#pragma region VertexData

float skyboxVertices[] = {        
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f,  1.0f
};

uint16_t skyboxIndiciesData[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35
};

#pragma endregion

void BufferManager::Init()
{
    // TODO: this shit is really wacky we need to abstract these mothefkas into their own class i think!!!!
#pragma region Skybox

    SkyboxVL.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .end();
    
    SkyboxVBO = bgfx::createVertexBuffer(bgfx::makeRef(skyboxVertices, sizeof(float) * 108), SkyboxVL);
    SkyboxEBO = bgfx::createIndexBuffer(bgfx::makeRef(skyboxIndiciesData, sizeof(uint16_t) * 36));

#pragma endregion

#pragma region StaticMesh

    StaticMeshVL.begin()
        // NORMALS ARE NOT WEVEN NEEDED LOOOOL
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .end();

#pragma endregion

#pragma region SkinnedMeshVL

    SkinnedMeshVL.begin()
        // NORMALS ARE NOT WEVEN NEEDED LOOOOL
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
         // TODO: can be just a fucking float if needed be...
        //.add(bgfx::Attrib::TexCoord1, 4, bgfx::AttribType::Int16)
        .add(bgfx::Attrib::Indices, 4, bgfx::AttribType::Float) // BLENDINDICES
        .add(bgfx::Attrib::Weight, 4, bgfx::AttribType::Float) // BLENDWEIGHT
        .end();

    /*
    layout (location = 3) in ivec4 BoneIDs;
    layout (location = 4) in vec4 Weights;
    */

#pragma endregion

}

void BufferManager::Shutdown() {}
