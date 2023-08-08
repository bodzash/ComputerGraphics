#pragma once
#include <vector>
#include "glm.hpp"
#include "Actor.h"
#include "ProjCamera.h"

class Level
{
public:
    glm::mat4 Model{1.0f};
    glm::mat4 View{1.0f};
    glm::mat4 Proj{1.0f};

    void Load(const char* path);
    void OnUpdate();
    void OnRender();
    inline ProjCamera& GetCamera() { return m_Camera; }
    inline const ProjCamera& GetCamera() const { return m_Camera; }

    // Implement forward args
    template<class T>
    T* CreateActor(/* forward args */);

private:
    std::vector<Actor*> m_Actors;
    ProjCamera m_Camera;
    // this need to be thought about or something lol
    // Texture m_Skybox;
    // DirectionalLight m_DirLight;
};