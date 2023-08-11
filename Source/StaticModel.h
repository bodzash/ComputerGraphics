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
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    // Tangent
    // Bitangent
};

struct StaticMesh
{
    std::vector<StaticVertex> Vertices;
    std::vector<uint16_t> Indices;

    bgfx::VertexBufferHandle VBO;
    bgfx::IndexBufferHandle EBO;

    bgfx::TextureHandle Diffuse;
    bgfx::TextureHandle Specular;
    //bgfx::TextureHandle Normal;
    //bgfx::TextureHandle Emission; // slash AO

    void SetupBuffers()
    {
        VBO = bgfx::createVertexBuffer(bgfx::makeRef(Vertices.data(), sizeof(StaticVertex) * Vertices.size()), BufferManager::Get().StaticMeshVL);
        EBO = bgfx::createIndexBuffer(bgfx::makeRef(Indices.data(), sizeof(uint16_t) * Indices.size()));
    }

    // REMOVE AND DO NOT USE
    void Render()
    {
        /*
        // Bind buffers
        bgfx::setVertexBuffer(0, VBO);
        bgfx::setIndexBuffer(EBO);

        // Bind textures
        bgfx::setTexture(0, UniformManager::Get().Diffuse, Diffuse);
        bgfx::setTexture(1, UniformManager::Get().Specular, Specular);
        */

        // Submit
        // bgfx::submit(0, program);
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
