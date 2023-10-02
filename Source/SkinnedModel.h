#pragma once
#include <vector>
#include <unordered_map>
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
    float BoneIDs[4] = { 0.0f };
    float Weights[4] = { 0.0f } ;

    void AddBoneData(int boneId, float weight)
    {
        for (int i = 0; i < 4; i++)
        {
            if (Weights[i] == 0.0f)
            {
                BoneIDs[i] = boneId;
                Weights[i] = weight;
                return;
            }
        }
    }
};

struct SkinnedVertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec4 BoneIDs;
    glm::vec4 Weights;
};

struct SkinnedMesh
{
    std::vector<SkinnedVertex> Vertices;
    std::vector<uint16_t> Indices;

    bgfx::VertexBufferHandle VBO = { bgfx::kInvalidHandle };
    bgfx::IndexBufferHandle EBO = { bgfx::kInvalidHandle };

    bgfx::TextureHandle Diffuse = { bgfx::kInvalidHandle };

    void SetupBuffers()
    {
        VBO = bgfx::createVertexBuffer(bgfx::makeRef(Vertices.data(), sizeof(SkinnedVertex) * Vertices.size()), BufferManager::Get().SkinnedMeshVL);
        EBO = bgfx::createIndexBuffer(bgfx::makeRef(Indices.data(), sizeof(uint16_t) * Indices.size()));
    }

    // TODO: may not implement thisway but lets set up bonesies :)
    void SetupBones()
    {
        
    }
};

struct BoneInfo
{
    aiMatrix4x4 OffsetMatrix;
    aiMatrix4x4 FinalTransformation; // should be identity by default

    BoneInfo(const aiMatrix4x4& offset)
    {
        OffsetMatrix = offset;
        // FinalTransformation{0.0f}; // should be identity by default

        // Set matrix to 0 lol, this is retarded    
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                FinalTransformation[x][y] = 0.0f;
            }
        }
    }
};

class SkinnedModel
{
public:
    std::vector<SkinnedMesh> Meshes;
    
    std::string Directory;

    glm::mat4 GlobalInverseTransform;

    Assimp::Importer Importer;
    const aiScene* Scene = nullptr;
    std::vector<BoneInfo> m_BoneInfo;

    std::vector<VertexBoneData> VertexToBones;
    std::vector<int> MeshBaseVertex;
    std::unordered_map<std::string, unsigned int> BoneNameToIndexMap;

    SkinnedModel() = default;
    SkinnedModel(const std::string& path);
    void GetBoneTransforms(std::vector<glm::mat4>& transforms);

private:
    void Load(const std::string& path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    SkinnedMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    void ProcessMeshBones(int meshIndex, const aiMesh* mesh);
    void ProcessSingleBone(int meshIndex, const aiBone* bone);
    void ReadNodeHierarchy(const aiNode* node, const aiMatrix4x4& parentTransform);

    int GetBoneId(const aiBone* bone)
    {
        int boneId = 0;
        std::string name(bone->mName.C_Str());

        if (BoneNameToIndexMap.find(name) == BoneNameToIndexMap.end())
        {
            boneId = BoneNameToIndexMap.size();
            BoneNameToIndexMap[name] = boneId;
        }
        else
        {
            boneId = BoneNameToIndexMap[name];
        }

        return boneId;
    }
    
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