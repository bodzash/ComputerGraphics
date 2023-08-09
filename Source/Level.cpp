#include "Level.h"

void Level::OnUpdate()
{
    for (Actor* actor : m_Actors)
    {
        actor->OnUpdate();
    }
}