#pragma once
#include <string>
#include "bgfx/bgfx.h"
#include "assimp/scene.h"


struct Texture
{
    bgfx::TextureHandle Handle;
    std::string Path = "";
    aiTextureType Type = aiTextureType::aiTextureType_UNKNOWN;

    Texture() = default;
    Texture(std::string path, aiTextureType type = aiTextureType::aiTextureType_UNKNOWN);
    ~Texture();
};