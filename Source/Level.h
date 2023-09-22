#pragma once
#include <vector>
#include <string>
#include "glm.hpp"
#include "Actor.h"
#include "PerspectiveCamera.h"

class Level
{
public:
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

    std::string m_Skybox = "Content/Textures/Skyboxes/SkyboxDay.dds";

private:
    std::vector<Actor*> m_Actors;
    PerspectiveCamera m_Camera;
    //PersCamera m_MainCamera;
    //PersCamera m_WeaponCamera;
    //OrthoCamera m_ShadowCamera;
    // Skybox things is need to be thought about lol :D
    // DirectionalLight m_DirLight;
};