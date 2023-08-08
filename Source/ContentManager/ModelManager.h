#pragma once
#include <unordered_map>


// TEMPORARY:
class Model;

class ModelManager
{
public:
    void Init();
    void Shutdown();
    Model GetByName(const char* name);

private:
    
};