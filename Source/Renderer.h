#pragma once
#include <string>
#include <stdint.h>
#include "bgfx/bgfx.h"
#include "glm.hpp"
#include "Window.h"

class Renderer
{
public:
    glm::mat4 Model{1.0f};
    glm::mat4 View{1.0f};
    glm::mat4 Proj{1.0f};

    Renderer() = default;
    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;

    static Renderer& Get()
    {
        static Renderer s_Instance;
        return s_Instance;
    }

    void Init(void* nativeWindowHandle);
    void Shutdown();
    void ResizeViews(uint16_t width, uint16_t height);

   void UpdateCamera();
   void CullRenderables(); // aka particles, decals, meshes

    void BeginRender();
    static void RenderSkyboxTest(const std::string& skybox) { Renderer::Get().RenderSkybox(skybox); }
    void RenderSkybox(const std::string& skybox);
    void RenderParticles();
    void RenderDecals();
    void RenderLightBakedMeshes();
    void RenderStaticMeshes();
    void RenderSkinnedMeshes();
    void RenderFirstPersonMeshes();
    void RenderUserInterface();
    void EndRender();

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