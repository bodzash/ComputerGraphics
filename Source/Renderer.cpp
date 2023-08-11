#include "Renderer.h"
#include "ContentManagers/UniformManager.h"
#include "ContentManagers/ShaderManager.h"
#include "ContentManagers/TextureManager.h"
#include "ContentManagers/BufferManager.h"

void Renderer::Init(void* nativeWindowHandle)
{
    bgfx::Init init;
    init.platformData.nwh = nativeWindowHandle;
    init.type = bgfx::RendererType::Direct3D9; // should detect automagically later
    // TODO: should load from Settings file and generate one if it doenst exist
    init.resolution.width = 800;
    init.resolution.height = 600;
    init.resolution.reset = m_Reset;

    // Init BGFX
    bgfx::init(init);

    // DEBUG
    //bgfx::setDebug(BGFX_DEBUG_STATS);

    // Set up views
    ResizeViews(800, 600);
}

void Renderer::Shutdown()
{
    bgfx::shutdown();
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

void Renderer::BeginRender()
{
    // Setup camera shit
}

void Renderer::RenderSkybox(const std::string &skybox)
{
    bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LEQUAL);

    glm::mat4 skyboxView = glm::mat4(glm::mat3(View));
    glm::mat4 skyboxViewProj = Proj * skyboxView;

    bgfx::setUniform(UniformManager::Get().Model, &Model);
    bgfx::setUniform(UniformManager::Get().ProjView, &skyboxViewProj);

    bgfx::setVertexBuffer(0, BufferManager::Get().SkyboxVBO);
    bgfx::setIndexBuffer(BufferManager::Get().SkyboxEBO);
    bgfx::setTexture(0, UniformManager::Get().Diffuse, TextureManager::Get().GetHandleByPath(skybox));
    bgfx::submit(m_GeometryView, ShaderManager::Get().Skybox);
}

void Renderer::RenderStaticMeshes()
{
    /*
    bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CW);

    // foreach StaticActor do the stuff bellow:

    glm::mat4 viewProj = Proj * View;
    glm::mat4 inverseModel = glm::inverse(Model);
    glm::vec4 viewPos(camera.pos, 1.0f); 
    // get dirlight data
    // get material info

    bgfx::setUniform(UniformManager::Get().ProjView, &viewProj);
    bgfx::setUniform(UniformManager::Get().Model, &Model);
    bgfx::setUniform(UniformManager::Get().InverseModel, &inverseModel);
    //bgfx::setUniform(UniformManager::Get().Material, &materialData.Shininess);
    //bgfx::setUniform(UniformManager::Get().DirLight, &dlightData, 4);
    bgfx::setUniform(UniformManager::Get().ViewPosition, &viewPos);

    mdl.Render(m_GeometryView, UniformManager::Get().Diffuse, UniformManager::Get().Specular,
        ShaderManager::Get().StaticMesh);
    */
}

void Renderer::EndRender()
{
    bgfx::frame();
}
