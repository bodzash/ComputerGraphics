#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <cstdint>
#include <bgfx/bgfx.h>
#include <glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/quaternion.h>
#include <assimp/vector3.h>
#include <assimp/matrix4x4.h>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include "ContentManagers/BufferManager.h"
#include "AssimpUtil.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    float BoneIDs[MAX_BONE_INFLUENCE];
    float Weights[MAX_BONE_INFLUENCE];
};

struct BoneInfo
{
    int ID;
    glm::mat4 Offset;
};

struct Mesh
{
    std::vector<Vertex> Vertices;
    std::vector<uint16_t> Indices;

    bgfx::VertexBufferHandle VBO = { bgfx::kInvalidHandle };
    bgfx::IndexBufferHandle EBO = { bgfx::kInvalidHandle };

    bgfx::TextureHandle Diffuse = { bgfx::kInvalidHandle };

    void SetupBuffers()
    {
        VBO = bgfx::createVertexBuffer(bgfx::makeRef(Vertices.data(), sizeof(Vertex) * Vertices.size()), BufferManager::Get().SkinnedMeshVL);
        EBO = bgfx::createIndexBuffer(bgfx::makeRef(Indices.data(), sizeof(uint16_t) * Indices.size()));
    }
};

class Model
{
public:
    std::vector<Mesh> Meshes;
    std::string Directory;
	std::unordered_map<std::string, BoneInfo> m_BoneInfoMap;
    int m_BoneCounter = 0;

    Model() = default;
    Model(const std::string& path);

    auto& GetBoneInfoMap() { return m_BoneInfoMap; }
    int& GetBoneCount() { return m_BoneCounter; } 

    void SetVertexBoneDataToDefault(Vertex& vertex);
    void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
    void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);

private:
    void Load(const std::string& path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
};