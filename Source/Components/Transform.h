#pragma once
#include "glm.hpp"

struct CTransform
{
    glm::vec3 Translation{ 1.0f };
    glm::vec3 Rotation{ 1.0f };
    glm::vec3 Scale{ 1.0f };
};