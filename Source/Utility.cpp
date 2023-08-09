#include "Utility.h"
#include <cassert>

namespace Utility
{

    const bgfx::Memory* LoadBinaryData(const std::string& path)
    {
        FILE* file = fopen(path.c_str(), "rb");
        assert(file && "Can not read file, maybe file is missing or there is a typo");
        fseek(file, 0, SEEK_END);
        const bgfx::Memory* mem = bgfx::alloc(ftell(file));
        fseek(file, 0, SEEK_SET);
        fread(mem->data, mem->size, 1, file);
        fclose(file);

        return mem;
    }

    const bgfx::Memory* LoadBinaryData(const char* path)
    {
        FILE* file = fopen(path, "rb");
        assert(file && "Can not read file, maybe file is missing or there is a typo");
        fseek(file, 0, SEEK_END);
        const bgfx::Memory* mem = bgfx::alloc(ftell(file));
        fseek(file, 0, SEEK_SET);
        fread(mem->data, mem->size, 1, file);
        fclose(file);

        return mem;
    }

}