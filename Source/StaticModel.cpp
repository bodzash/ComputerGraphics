#include "StaticModel.h"
#include <iostream>

StaticModel::StaticModel(const std::string &path)
{
    Load(path);

    for (auto& mesh : Meshes)
    {
        mesh.SetupBuffers();
    }
}

void StaticModel::Load(const std::string &path)
{
    // aiProcess_FlipUVs <- if directx use this lol
    Assimp::Importer importer;
    
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "Model loading error:" << importer.GetErrorString() << '\n';
        return;
    }

    ProcessNode(scene->mRootNode, scene);
}

void StaticModel::ProcessNode(aiNode* node, const aiScene* scene)
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

StaticMesh StaticModel::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    StaticMesh loadingMesh;

    // Process vertecies
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        StaticVertex vertex;

        // Positions
        vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

        // Normals
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

    return loadingMesh;
}