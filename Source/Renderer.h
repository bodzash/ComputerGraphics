#pragma once
#include <stdint.h>
#include <bgfx/bgfx.h>
#include "Window.h"

class Renderer
{
public:
    void Init();
    void Shutdown();
    void Update();
    void ResizeViews(uint16_t width, uint16_t height);

    /*
    // Unused
    void BeginRender();
    void EndRender();
    */


protected:
    // Should take in a skybox texture ref
    void RenderSkybox();
    void RenderStaticMesh();
    void RenderSkinnedMesh();
    // ...

private:
    uint64_t m_State = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A;
    uint32_t m_Reset = 0 | BGFX_RESET_VSYNC | BGFX_RESET_FLUSH_AFTER_RENDER;

    const bgfx::ViewId m_GeometryView = 0;
    /*
    const bgfx::ViewId GEOMETRY_PASS = 1;
    const bgfx::ViewId LIGHTING_PASS = 2;
    */

};