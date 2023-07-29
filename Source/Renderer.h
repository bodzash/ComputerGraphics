#pragma once
#include <stdint.h>
#include "bgfx/bgfx.h"
#include "Window.h"

class Renderer
{
public:
    void Init();
    void Shutdown();

private:
    uint64_t m_State = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A;
};