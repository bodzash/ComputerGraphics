#include "Renderer.h"

void Renderer::Init()
{
    bgfx::Init init;
    init.platformData.nwh = Window::GetNativeWindowHandle();
    init.type = bgfx::RendererType::Direct3D9;
    init.resolution.width = 800;
    init.resolution.height = 600;
    init.resolution.reset = BGFX_RESET_VSYNC | BGFX_RESET_FLUSH_AFTER_RENDER;

    bgfx::init(init);
}

void Renderer::Shutdown()
{
    bgfx::shutdown();
}