#pragma once

class Actor
{
public:
    void OnUpdate() {}
    void OnRender() {}

protected:
    // TODO:
    Actor** m_Children;
};