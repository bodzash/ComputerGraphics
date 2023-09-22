#include "Texture.h"
#include "Utility.h"
#include "iostream"

Texture::Texture(std::string path, aiTextureType type)
    : Path(path), Type(type)
{
    const bgfx::Memory* mem = Utility::LoadBinaryData(path);
    // TODO: sampling setting
    Handle = bgfx::createTexture(mem, BGFX_TEXTURE_NONE | BGFX_SAMPLER_UVW_CLAMP);
}

Texture::~Texture()
{
    std::cout << "Destroyed: " << Path << '\n';
    bgfx::destroy(Handle);
}