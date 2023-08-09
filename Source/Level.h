#pragma once
#include <vector>
#include "glm.hpp"
#include "Actor.h"
#include "PerspectiveCamera.h"

class Level
{
public:
    // TODO: mode to renderer lol
    glm::mat4 Model{1.0f};
    glm::mat4 View{1.0f};
    glm::mat4 Proj{1.0f};

    void Load(const char* path);
    void OnUpdate();
    void OnRender();
    inline PerspectiveCamera& GetCamera() { return m_Camera; }
    inline const PerspectiveCamera& GetCamera() const { return m_Camera; }

    // Implement forward args
    template <class T>
    T* CreateActor()
    {
        T* actor = new T();
        m_Actors.push_back(actor);
        return actor;
    }
private:
    std::vector<Actor*> m_Actors;
    PerspectiveCamera m_Camera;
    // this need to be thought about or something lol
    // Texture m_Skybox;
    // DirectionalLight m_DirLight;
};

