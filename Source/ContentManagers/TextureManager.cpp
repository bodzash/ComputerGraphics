#include "TextureManager.h"
#include "iostream"

void TextureManager::Load(std::string path)
{
    if (m_Textures.count(path) == 1)
    {
        std::cout << "Trying to load a duplicate: " << m_Textures[path].Path << '\n';
        std::cout << "Not loading the same asset twice!\n";
        return;
    }

    m_Textures.emplace(path, path);
    std::cout << "Loaded: " << m_Textures[path].Path << '\n';
}

bgfx::TextureHandle TextureManager::GetHandleByPath(std::string path)
{
    return m_Textures[path].Handle;
}

void TextureManager::Clear()
{
    m_Textures.clear();
}
