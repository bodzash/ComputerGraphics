#include "Texture.h"
#include "Utility.h"

Texture::Texture(const std::string& path)
{
    // TODO: sampling setting
    const bgfx::Memory* mem = Utility::LoadBinaryData(path);
    Handle = bgfx::createTexture(mem, BGFX_TEXTURE_NONE);
}

Texture::~Texture()
{
    //printf("Texture removed from VRAM: %d", Handle.id);
    bgfx::destroy(Handle);
}