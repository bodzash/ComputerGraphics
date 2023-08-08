#pragma once
#include <vector>
#include "Actor.h"

class Level
{
public:
    void Load(const char* path);
    // TODO: should take in a delta time
    void OnUpdate();
    void OnRender();

    // Implement forward args
    template<class T>
    T* CreateActor(/* forward args */);

private:
    std::vector<Actor*> m_Actors;
};