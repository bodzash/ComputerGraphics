#include "ModelManager.h"
#include <iostream>

void ModelManager::Init()
{

}

void ModelManager::Shutdown()
{

}

StaticModel& ModelManager::LoadStatic(std::string path)
{
    if (m_StaticModels.count(path) == 1)
    {
        std::cout << "Trying to load a duplicate: " << path << '\n';
        std::cout << "Not loading the same static model twice!\n";
        return m_StaticModels[path];
    }

    m_StaticModels.emplace(path, path);
    std::cout << "Loaded static model:" << path << '\n';

    return m_StaticModels[path];
}