#pragma once
#include <string>
#include "bgfx/bgfx.h"

struct Texture
{
    bgfx::TextureHandle Handle;

    Texture(const std::string& path);
    ~Texture();
};