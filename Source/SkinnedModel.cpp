#include "SkinnedModel.h"
#include <iostream>
#include "ContentManagers/TextureManager.h"

SkinnedModel::SkinnedModel(const std::string &path)
{
    Directory = path;
    Directory.resize(path.find_last_of("/") + 1);

    Load(path);

    for (auto& mesh : Meshes)
    {
        mesh.SetupBuffers();
    }
}

void SkinnedModel::Load(const std::string &path)
{
    Assimp::Importer importer;
    
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "Model loading error:" << importer.GetErrorString() << '\n';
        return;
    }

    // NOTE: that reverse maybe not needed
    GlobalInverseTransform = AssimpToGlmMatrix(scene->mRootNode->mTransformation.Inverse());

    ProcessNode(scene->mRootNode, scene);
}

void SkinnedModel::ProcessNode(aiNode* node, const aiScene* scene)
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

SkinnedMesh SkinnedModel::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    SkinnedMesh loadingMesh;

    //mesh->mBones[0].

    for (unsigned int i = 0; i < mesh->mNumBones; i++)
    {
        std::cout << mesh->mBones[i]->mName.C_Str() << '\n';
    }

    // Process vertecies
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        SkinnedVertex vertex;

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

    return loadingMesh;
}


glm::mat4 SkinnedModel::AssimpToGlmMatrix(aiMatrix4x4 mat)
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