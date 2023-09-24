#include "Renderer.h"
#include "ContentManagers/UniformManager.h"
#include "ContentManagers/ShaderManager.h"
#include "ContentManagers/TextureManager.h"
#include "ContentManagers/BufferManager.h"

#include "Actor.h"

Renderer::Renderer(Level& level)
    : m_Level(level)
{
}

void Renderer::Init(void *nativeWindowHandle)
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
    bgfx::setViewClear(m_GeometryView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x8A8E8CFF, 1.0f);
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

void Renderer::RenderStaticMeshes(const std::vector<Actor*>& actors)
{
    bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CW);

    // Get the level/scene camera here dawg
    //PerspectiveCamera m_Camera;

    // Set data
    glm::mat4 viewProj = Proj * View;
    //glm::vec4 viewPos(m_Camera.Position, 1.0f);

    // Set uniforms
    bgfx::setUniform(UniformManager::Get().ProjView, &viewProj);
    //bgfx::setUniform(UniformManager::Get().ViewPosition, &viewPos);
    //bgfx::setUniform(UniformManager::Get().DirLight, &dlightData, 4);

    /*
    for (Actor* actor : actors)
    {
        // set model here using the transform of the actor, rawr :D
        glm::mat4 inverseModel = glm::inverse(Model);

        bgfx::setUniform(UniformManager::Get().Model, &Model);
        bgfx::setUniform(UniformManager::Get().InverseModel, &inverseModel);
        //bgfx::setUniform(UniformManager::Get().Material, &materialData.Shininess);

        // Bind Buffers
        bgfx::setVertexBuffer(0, mesh.VBO);
        bgfx::setIndexBuffer(mesh.EBO);

        // Bind textures
        bgfx::setTexture(0, UniformManager::Get().Diffuse, mesh.Diffuse);
        bgfx::setTexture(1, UniformManager::Get().Specular, mesh.Specular);

        // Submit
        bgfx::submit(0, ShaderManager::Get().StaticMesh);
    }
    */
}

void Renderer::EndRender()
{
    bgfx::frame();
}
