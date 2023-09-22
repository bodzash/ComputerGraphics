#include "TextureManager.h"
#include "iostream"

bgfx::TextureHandle TextureManager::Load(std::string path)
{
    if (m_Textures.count(path) == 1)
    {
        std::cout << "Trying to load a duplicate: " << m_Textures[path].Path << '\n';
        std::cout << "Not loading the same texture twice!\n";
        return m_Textures[path].Handle;
    }

    m_Textures.emplace(path, path);
    std::cout << "Loaded texture: " << m_Textures[path].Path << '\n';

    return m_Textures[path].Handle;
}

bool TextureManager::IsAlreadyLoaded(std::string path)
{
    return m_Textures.count(path) == 1;
}

bgfx::TextureHandle TextureManager::GetHandleByPath(std::string path)
{
    return m_Textures[path].Handle;
}

void TextureManager::Clear()
{
    m_Textures.clear();
}
