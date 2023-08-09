#pragma once
#include <string>
#include <unordered_map>
#include "../Texture.h"

// TODO: optimize this by using string views and const char*'s
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

    //void Init();
    void Shutdown() { Clear(); }
    void Load(std::string path);
    bgfx::TextureHandle GetHandleByPath(std::string path);
    void Clear();
    

private:
    // maybe const char* as key? lol
    // even maybe should store a pointer or something idk tbh
    std::unordered_map<std::string, Texture> m_Textures;
};