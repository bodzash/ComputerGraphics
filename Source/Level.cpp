#include "Level.h"
#include "Renderer.h"
#include "ContentManagers/TextureManager.h"

void Level::Load(const char* path)
{
    TextureManager::Get().Load(m_Skybox);
}

void Level::OnUpdate()
{
  for (Actor* actor : m_Actors)
  {
    actor->OnUpdate();
  }
}

// TODO: MAYBE THE LEVEL CAN OWN THE RENDERER, SOUNDS RETARDED BUT A POSSIBILITY

void Level::OnRender()
{
    Renderer::Get().BeginRender();

    //Renderer::Get().RenderStaticMeshes(m_Actors);

    // Render shadowmap?

    // Render particles?

    //Renderer::Get().RenderSkybox(m_Skybox);
    Renderer::RenderSkyboxTest(m_Skybox);

    // Render weapon that is in the hand of the player (pass every depth test)

    // Post Processing

    // Render UI (pass depth test for UI FBO)

    // EndRender
    Renderer::Get().EndRender();
}