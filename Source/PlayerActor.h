#pragma once
#include <iostream>
#include "glm.hpp"
#include "Actor.h"

class PlayerActor : public Actor
{
public:
    // Model* CurrentModel;
    // Model* CurrentModel = ModelManager::GetModel("Angel/Skel_VoG.dae");
    // std::unique_ptr<Model> CurrentModel = ModelManager::GetModel("Angel/Skel_VoG.dae");

    void OnUpdate() override
    {
        //std::cout << "PlayerActor::OnUpdate\n";
    }

protected:

private:

};
