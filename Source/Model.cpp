#include "Model.h"
#include <iostream>

Model::Model(const std::string &path)
{
    Directory = path;
    Directory.resize(path.find_last_of("/") + 1);

    Load(path);

    for (auto& mesh : Meshes)
    {
        mesh.SetupBuffers();
    }
}

void Model::Load(const std::string &path)
{
    Assimp::Importer importer;
    
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "Model loading error:" << importer.GetErrorString() << '\n';
        return;
    }

    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    for (int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        Meshes.push_back(ProcessMesh(mesh, scene));
    }
    
    for (int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    Mesh loadingMesh;

    // Process vertecies
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        SetVertexBoneDataToDefault(vertex);

        // Positions
        vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

        // Normals TODO: remove this
        if (mesh->HasNormals())
            vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        else
            vertex.Normal = glm::vec3(1.0f);

        // Texture coordinates
        if (mesh->mTextureCoords[0])
        {
            vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        else
            vertex.TexCoords = glm::vec2(0.0f);

        // Add a vertex to mesh
        loadingMesh.Vertices.push_back(vertex);
    }

    // Process indices
    for (int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        for (int j = 0; j < face.mNumIndices; j++)
            loadingMesh.Indices.push_back(face.mIndices[j]);
    }

    ExtractBoneWeightForVertices(loadingMesh.Vertices, mesh, scene);

    return loadingMesh;
}

void Model::SetVertexBoneDataToDefault(Vertex& vertex)
{
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        vertex.BoneIDs[i] = -1;
        vertex.Weights[i] = 0.0f;
    }
}

void Model::SetVertexBoneData(Vertex& vertex, int boneID, float weight)
{
    for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
    {
        if (vertex.BoneIDs[i] < 0)
        {
            vertex.Weights[i] = weight;
            vertex.BoneIDs[i] = boneID;
            break;
        }
    }
}

void Model::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
{
    auto& boneInfoMap = m_BoneInfoMap;
    int& boneCount = m_BoneCounter;

    for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
    {
        int boneID = -1;
        std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
        if (boneInfoMap.find(boneName) == boneInfoMap.end())
        {
            BoneInfo newBoneInfo;
            newBoneInfo.ID = boneCount;
            newBoneInfo.Offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
            boneInfoMap[boneName] = newBoneInfo;
            boneID = boneCount;
            boneCount++;
        }
        else
        {
            boneID = boneInfoMap[boneName].ID;
        }
        assert(boneID != -1);
        auto weights = mesh->mBones[boneIndex]->mWeights;
        int numWeights = mesh->mBones[boneIndex]->mNumWeights;

        for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
        {
            int vertexId = weights[weightIndex].mVertexId;
            float weight = weights[weightIndex].mWeight;
            assert(vertexId <= vertices.size());
            SetVertexBoneData(vertices[vertexId], boneID, weight);
        }
    }
}