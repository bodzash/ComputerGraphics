#pragma once
#include "glm.hpp"

class PerspectiveCamera final
{
public:
    glm::vec3 Position = {0.0f, 0.0f, 0.0f};
    glm::vec3 Orientation = {1.0f, 0.0f, 0.0f};
    glm::vec3 Up = {0.0f, 1.0f, 0.0f};

    //inline glm::vec3& GetPosition() { return m_Position; }
    //inline const glm::vec3& GetPosition() const { return m_Position; }
private:

};