#include "Model.h"
#include <iostream>
#include "ContentManagers/TextureManager.h"

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
    //loadingMesh.Vertices.resize(mesh->mNumVertices);

    // Process vertecies
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        SetVertexBoneDataToDefault(vertex);

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
        //loadingMesh.Vertices[i] = vertex;
    }

    // Process indices
    for (int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        for (int j = 0; j < face.mNumIndices; j++)
            loadingMesh.Indices.push_back(face.mIndices[j]);
    }

    ExtractBoneWeightForVertices(loadingMesh.Vertices, mesh, scene);
    /*
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
    */

    return loadingMesh;
}