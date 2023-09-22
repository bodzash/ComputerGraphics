#include "StaticModel.h"
#include <iostream>
#include "ContentManagers/TextureManager.h"

StaticModel::StaticModel(const std::string &path)
{
    Directory = path;
    Directory.resize(path.find_last_of("/") + 1);

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
    
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "Model loading error:" << importer.GetErrorString() << '\n';
        return;
    }

    ProcessNode(scene->mRootNode, scene);
}

void StaticModel::ProcessNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        Meshes.push_back(ProcessMesh(mesh, scene));
    }
    
    for (unsigned int i = 0; i < node->mNumChildren; i++)
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

        /*
        DO A CHECK FOR TANG AND BITANG LIKE ABOVE :D
        // Tangent
        vertex.Tangent = vector;
        glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);

        // Bitangent
        vertex.Bitangent = vector;
        glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
        */

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
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; j++)
            loadingMesh.Indices.push_back(face.mIndices[j]);
    }
    
    // Process textures
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    // Diffuse
    aiString diffusePath;
    material->GetTexture(aiTextureType_DIFFUSE, 0, &diffusePath);
    if (diffusePath.length)
    {
        std::string diffusePathStd(diffusePath.C_Str());
        diffusePathStd.replace(diffusePathStd.find(".png"), 4, ".dds");

        loadingMesh.Diffuse = TextureManager::Get().Load(Directory + diffusePathStd);
    }

    // Specular
    aiString specularPath;
    material->GetTexture(aiTextureType_SPECULAR, 0, &specularPath);
    if (specularPath.length)
    {
        std::string specularPathStd(specularPath.C_Str());
        specularPathStd.replace(specularPathStd.find(".png"), 4, ".dds");

        loadingMesh.Specular = TextureManager::Get().Load(Directory + specularPathStd);
    }

    return loadingMesh;
    //return StaticMesh(vertices, indices, textures);
}
