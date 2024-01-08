#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include "bgfx/bgfx.h"
#include "glm.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "ContentManagers/BufferManager.h"

struct StaticVertex
{
    glm::vec3 Position;
    glm::vec2 TexCoords;

    static VertexLayout Layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .end();
};

struct StaticMesh
{
    std::vector<StaticVertex> Vertices;
    std::vector<uint16_t> Indices;

    bgfx::VertexBufferHandle VBO = { bgfx::kInvalidHandle };
    bgfx::IndexBufferHandle EBO = { bgfx::kInvalidHandle };

    void SetupBuffers()
    {
        VBO = bgfx::createVertexBuffer(bgfx::makeRef(Vertices.data(), sizeof(StaticVertex) * Vertices.size()), BufferManager::Get().StaticMeshVL);
        EBO = bgfx::createIndexBuffer(bgfx::makeRef(Indices.data(), sizeof(uint16_t) * Indices.size()));
    }
};

class StaticModel
{
public:
    std::vector<StaticMesh> Meshes;

    StaticModel() = default;
    StaticModel(const std::string& path);
private:
    void Load(const std::string& path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    StaticMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
};
