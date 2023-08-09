#pragma once
#include <stdint.h>
#include <bgfx/bgfx.h>
#include "Window.h"

class Renderer
{
public:
    void Init(void* nativeWindowHandle);
    void Shutdown();
    void Render();
    void ResizeViews(uint16_t width, uint16_t height);

    /*
    // Unused for now i guess
    void BeginRender();
    void EndRender();
    */

   void UpdateCamera();
   void CullRenderables(); // aka particles, decals, meshes

protected:
    void RenderSkybox();
    void RenderParticle();
    void RenderDecal();
    void RenderLightBakedMesh();
    void RenderStaticMesh();
    void RenderSkinnedMesh();
    void RenderFirstPersonMesh();

private:
    uint64_t m_State = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A;
    uint32_t m_Reset = 0 | BGFX_RESET_VSYNC | BGFX_RESET_FLUSH_AFTER_RENDER;

    const bgfx::ViewId m_GeometryView = 0;
    /*
    const bgfx::ViewId GEOMETRY_PASS = 1;
    const bgfx::ViewId LIGHTING_PASS = 2;
    const bgfx::ViewId POSTPROCESS_PASS = 3;
    // ...
    */

};