#include "Utility.h"

namespace Utility
{

    const bgfx::Memory* LoadBinaryData(const std::string& path)
    {
        FILE* file = fopen(path.c_str(), "rb");
        fseek(file, 0, SEEK_END);
        const bgfx::Memory* mem = bgfx::alloc(ftell(file));
        fseek(file, 0, SEEK_SET);
        fread(mem->data, mem->size, 1, file);
        fclose(file);

        return mem;
    }

}