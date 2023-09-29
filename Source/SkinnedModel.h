#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include "bgfx/bgfx.h"
#include "glm.hpp"
#include "gtc/quaternion.hpp"
#include "gtx/quaternion.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "ContentManagers/BufferManager.h"

#define NUM_BONES_PER_VERTEX 4

struct VertexBoneData
{
    int16_t BoneIDs[NUM_BONES_PER_VERTEX];
    float Weights[NUM_BONES_PER_VERTEX];
};

struct SkinnedVertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    int16_t BoneIDs[NUM_BONES_PER_VERTEX];
    float Weights[NUM_BONES_PER_VERTEX];
};

struct SkinnedMesh
{
    std::vector<SkinnedVertex> Vertices;
    std::vector<uint16_t> Indices;
    std::vector<VertexBoneData> Bones;

    bgfx::VertexBufferHandle VBO = { bgfx::kInvalidHandle };
    bgfx::IndexBufferHandle EBO = { bgfx::kInvalidHandle };

    bgfx::TextureHandle Diffuse = { bgfx::kInvalidHandle };

    void SetupBuffers()
    {
        VBO = bgfx::createVertexBuffer(bgfx::makeRef(Vertices.data(), sizeof(SkinnedVertex) * Vertices.size()), BufferManager::Get().SkinnedMeshVL);
        EBO = bgfx::createIndexBuffer(bgfx::makeRef(Indices.data(), sizeof(uint16_t) * Indices.size()));
    }
};

class SkinnedModel
{
public:
    std::vector<SkinnedMesh> Meshes;
    
    std::string Directory;
    glm::mat4 GlobalInverseTransform;

    SkinnedModel() = default;
    SkinnedModel(const std::string& path);

private:
    void Load(const std::string& path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    SkinnedMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    
    glm::mat4 AssimpToGlmMatrix(aiMatrix4x4 mat);
};

/*
glm::mat4 AssimpToGlmMatrix(aiMatrix4x4 mat)
{
	glm::mat4 m;

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			m[x][y] = mat[y][x];
		}
	}

	return m;
}

glm::vec3 AssimpToGlmVec3(aiVector3D vec)
{
    return glm::vec3(vec.x, vec.y, vec.z);
}

glm::quat AssimpToGlmQuat(aiQuaternion quat) {
	glm::quat q;

	q.x = quat.x;
	q.y = quat.y;
	q.z = quat.z;
	q.w = quat.w;

	return q;
}
*/