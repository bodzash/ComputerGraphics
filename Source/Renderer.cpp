#include "Renderer.h"

void Renderer::Init(void* nativeWindowHandle)
{
    bgfx::Init init;
    init.platformData.nwh = nativeWindowHandle;
    init.type = bgfx::RendererType::Direct3D9; // should detect automagically later
    // TODO: should load from Settings file
    init.resolution.width = 800;
    init.resolution.height = 600;
    init.resolution.reset = m_Reset;

    // Init BGFX
    bgfx::init(init);

    // DEBUG
    bgfx::setDebug(BGFX_DEBUG_STATS);

    // Set up views
    ResizeViews(800, 600);
}

void Renderer::Shutdown()
{
    bgfx::shutdown();
}

void Renderer::Render()
{
    // BeginRender

    // Render actors?

    // Render shadowmap?

    // Render particles?

    // Render Skybox and fail every depth test (so its behind everything)

    // Render weapon that is in the hand of the player

    // Post Processing

    // Render UI

    // EndRender
    bgfx::frame();
}

void Renderer::ResizeViews(uint16_t width, uint16_t height)
{
    // Resize framebuffers
    bgfx::setViewClear(m_GeometryView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f);
	bgfx::setViewRect(m_GeometryView, 0, 0, width, height);

    // TODO: 
    // screen-space fbo
    // shadow fbo
}
