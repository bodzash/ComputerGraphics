#pragma once

class Actor
{
public:
    void virtual OnUpdate() {}
    void virtual OnRender() {}

protected:
    // TODO:
    Actor** m_Children;
};