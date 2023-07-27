#pragma once
#include <string>
#include "bgfx/bgfx.h"

namespace Utility
{
    const bgfx::Memory* LoadBinaryData(const std::string& path);
}