#pragma once
#include <iostream>
#include "glm.hpp"
#include "Actor.h"

struct Model;

// TODO: MOVE THIS
struct CTransform
{
    glm::vec3 Translation{ 1.0f };
    glm::vec3 Rotation{ 1.0f };
    glm::vec3 Scale{ 1.0f };
};

class PlayerActor : public Actor
{
public:
    CTransform Transform;
    Model* CurrentModel;
    // Model* CurrentModel = ModelManager::GetModel("Angel/Skel_VoG.dae");
    // std::unique_ptr<Model> CurrentModel = ModelManager::GetModel("Angel/Skel_VoG.dae");

    void OnUpdate() override
    {
        //std::cout << "PlayerActor::OnUpdate\n";
    }

protected:

private:

};
