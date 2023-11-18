#pragma once
#include <string>
#include <vector>
#include <map>
#include <glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "Bone.h"
#include "Model.h"

struct AssimpNodeData
{
    std::string Name;
    glm::mat4 Transformation;
    int ChildrenCount;
    std::vector<AssimpNodeData> Children;
};

struct AnimationData
{
    //std::string Name;
    float m_Duration;
    int m_TicksPerSecond;
};

class Animation
{
public:
    Animation() = default;
    Animation(const std::string& animationPath, Model* model);

    Bone* FindBone(const std::string& name);

    // GetNumAnims
    inline float GetTicksPerSecond() { return m_TicksPerSecond; }
    inline float GetDuration() { return m_Duration;}
    inline const AssimpNodeData& GetRootNode() { return m_RootNode; }
    inline const std::map<std::string, BoneInfo>& GetBoneIDMap() { return m_BoneInfoMap; }

private:
    void ReadMissingBones(const aiAnimation* animation, Model& model);
    void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);

    std::vector<AnimationData> m_Animations;

    float m_Duration;
    int m_TicksPerSecond;
    std::vector<Bone> m_Bones;
    AssimpNodeData m_RootNode;
    std::map<std::string, BoneInfo> m_BoneInfoMap;
};