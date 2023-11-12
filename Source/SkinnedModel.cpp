#include "SkinnedModel.h"
#include <iostream>
#include "ContentManagers/TextureManager.h"

SkinnedModel::SkinnedModel(const std::string &path)
{
    Directory = path;
    Directory.resize(path.find_last_of("/") + 1);

    Load(path);

    // Set up bone data for meshes
    for (int i = 0; i < Meshes[0].Vertices.size(); i++)
    {
        //Meshes[0].Vertices[i].BoneIDs = glm::vec4(0.0f);
        Meshes[0].Vertices[i].BoneIDs = glm::vec4(VertexToBones[i].BoneIDs[0], VertexToBones[i].BoneIDs[1], VertexToBones[i].BoneIDs[2], VertexToBones[i].BoneIDs[3]);
        Meshes[0].Vertices[i].Weights = glm::vec4(VertexToBones[i].Weights[0], VertexToBones[i].Weights[1], VertexToBones[i].Weights[2], VertexToBones[i].Weights[3]);
    }

    for (auto& mesh : Meshes)
    {
        mesh.SetupBuffers();
    }
}

void SkinnedModel::Load(const std::string &path)
{
    //Assimp::Importer importer;
    
    Scene = Importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs);
    
    if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
    {
        std::cout << "Model loading error:" << Importer.GetErrorString() << '\n';
        return;
    }

    // NOTE: that reverse maybe not needed
    GlobalInverseTransform = AssimpToGlmMatrix(Scene->mRootNode->mTransformation.Inverse());

    ProcessNode(Scene->mRootNode, Scene);
}

void SkinnedModel::ProcessNode(aiNode* node, const aiScene* scene)
{
    int totalVert = 0;
    int totalInd = 0;
    int totalBones = 0;

    //MeshBaseVertex.resize(node->mNumMeshes);

    // What should be done is do meshes then do bones :)
    for (int i = 0; i < node->mNumMeshes; i++)
    {
        MeshBaseVertex.resize(node->mNumMeshes);

        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        Meshes.push_back(ProcessMesh(mesh, scene));

        int numVerts = mesh->mNumVertices;
        int numInds = mesh->mNumFaces * 3;
        int numBones = mesh->mNumBones;
        MeshBaseVertex[i] = totalVert;

        totalVert += numVerts;
        totalInd += numInds;
        totalBones = numBones;

        VertexToBones.resize(totalVert);

        if (mesh->HasBones())
            ProcessMeshBones(i, mesh);
    }
    
    for (int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }    
}

SkinnedMesh SkinnedModel::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    SkinnedMesh loadingMesh;

    // Process vertecies
    for (int i = 0; i < mesh->mNumVertices; i++)
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

        // Bones are done in somewhere else dawg :)

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

void SkinnedModel::ProcessMeshBones(int meshIndex, const aiMesh* mesh)
{
    for (int i = 0; i < mesh->mNumBones; i++)
    {
        ProcessSingleBone(meshIndex, mesh->mBones[i]);
    }
}

void SkinnedModel::ProcessSingleBone(int meshIndex, const aiBone* bone)
{
    int boneId = GetBoneId(bone);

    if (boneId == m_BoneInfo.size())
    {
        //BoneInfo bi(bone->mOffsetMatrix);
        //m_BoneInfo.push_back(bi);
        m_BoneInfo.emplace_back(bone->mOffsetMatrix);
    }

    for (int i = 0; i < bone->mNumWeights; i++)
    {
        // this changed sus
        const aiVertexWeight& vw = bone->mWeights[i];
        unsigned int globalVertexId = MeshBaseVertex[meshIndex] + vw.mVertexId;
        VertexToBones[globalVertexId].AddBoneData(boneId, vw.mWeight);
    }
}

void SkinnedModel::GetBoneTransforms(std::vector<glm::mat4>& transforms)
{
    transforms.resize(m_BoneInfo.size());

    aiMatrix4x4 identity;

    ReadNodeHierarchy(Scene->mRootNode, identity);

    for (int i = 0; i < m_BoneInfo.size(); i++)
    {
        transforms[i] = AssimpToGlmMatrix(m_BoneInfo[i].FinalTransformation);
    }
}

void SkinnedModel::ReadNodeHierarchy(const aiNode* node, const aiMatrix4x4& parentTransform)
{
    std::string nodeName(node->mName.data);

    aiMatrix4x4 nodeTransform = node->mTransformation;
    aiMatrix4x4 globalTransform = parentTransform * nodeTransform;

    if (BoneNameToIndexMap.find(nodeName) != BoneNameToIndexMap.end())
    {
        int boneIndex = BoneNameToIndexMap[nodeName];
        m_BoneInfo[boneIndex].FinalTransformation = globalTransform * m_BoneInfo[boneIndex].OffsetMatrix;
    }

    for (int i = 0; i < node->mNumChildren; i++)
    {
        ReadNodeHierarchy(node->mChildren[i], globalTransform);
    }
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