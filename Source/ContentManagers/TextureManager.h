#pragma once
#include <string>
#include <unordered_map>

class Texture;

class TextureManager final
{
public:
    TextureManager() = default;
    TextureManager(const TextureManager&) = delete;
    TextureManager(TextureManager&&) = delete;

    static TextureManager& Get()
    {
        static TextureManager s_Instance;
        return s_Instance;
    }

    void Init();
    void Shutdown();
    // Unloads everything
    void Clear();
    

private:
    // maybe const char* as key? lol
    //std::unordered_map<std::string, Texture> m_Textures;
};