#pragma once
#include <vector>

class KeyFrame;

class Animation
{
public:
    Animation(float length, std::vector<KeyFrame> keyFrames)
        : m_Length(length), m_KeyFrames(keyFrames) {}

    inline float GetLength() { return m_Length; }
    inline std::vector<KeyFrame>& GetKeyFrames() { return m_KeyFrames; }

private:
    float m_Length;
    std::vector<KeyFrame> m_KeyFrames;
};