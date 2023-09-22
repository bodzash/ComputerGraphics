#pragma once
#include <unordered_map>
#include "../StaticModel.h"

class ModelManager final
{
public:
    ModelManager() = default;
    ModelManager(const ModelManager&) = delete;
    ModelManager(ModelManager&&) = delete;

    static ModelManager& Get()
    {
        static ModelManager s_Instance;
        return s_Instance;
    }

    void Init();
    void Shutdown();
    StaticModel& LoadStatic(std::string path);
    //
    //Model GetByName(const char* name);
    //Model GetByPath(const char* path);

private:
    std::unordered_map<std::string, StaticModel> m_StaticModels;
};