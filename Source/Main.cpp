#include <iostream>
#include <vector>

#include <SDL.h>
#include <SDL_syswm.h>

#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include "bimg/bimg.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/rotate_vector.hpp>
#include <gtx/vector_angle.hpp>

// Let the "fun" begin
#include "Model.h"
#include "Animation.h"
#include "Animator.h"
#include "ContentManagers/UniformManager.h"
#include "ContentManagers/ShaderManager.h"
#include "ContentManagers/TextureManager.h"
#include "ContentManagers/BufferManager.h"
#include "ContentManagers/ModelManager.h"
#include "Utility.h"

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Fuck", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    int WINDOW_WIDTH = 800;
    int WINDOW_HEIGHT = 600;

    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    SDL_GetWindowWMInfo(window, &wmi);

    bgfx::Init init;
    init.platformData.ndt = nullptr;
    init.platformData.nwh = wmi.info.win.window;
    init.type = bgfx::RendererType::Direct3D9; // should detect automagically later
    init.resolution.width = 800;
    init.resolution.height = 600;
    init.resolution.reset = 0 | BGFX_RESET_VSYNC | BGFX_RESET_FLUSH_AFTER_RENDER;

    // Init BGFX
    bgfx::init(init);

    // Resize framebuffers
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x8A8E8CFF, 1.0f);
	bgfx::setViewRect(0, 0, 0, 800, 600);

    /*
    clog(bgfx::getCaps()->limits.maxDrawCalls);
    clog(bgfx::getCaps()->homogeneousDepth);
    clog(bgfx::getCaps()->limits.maxFBAttachments);
    clog(bgfx::getCaps()->limits.maxOcclusionQueries);
    std::cout << (bool)(bgfx::getCaps()->supported & BGFX_CAPS_RENDERER_MULTITHREADED) << '\n';
    std::cout << (bool)(bgfx::getCaps()->supported & BGFX_CAPS_OCCLUSION_QUERY) << '\n';
    std::cout << (bool)(bgfx::getCaps()->supported & BGFX_CAPS_TEXTURE_BLIT) << '\n';
    std::cout << (bool)(bgfx::getCaps()->supported & BGFX_CAPS_SWAP_CHAIN) << '\n';
    std::cout << (bool)(bgfx::getCaps()->supported & BGFX_CAPS_COMPUTE) << '\n';
    */

    // INIT ASSET MANAGERS, these shits should be remade
    ShaderManager::Get().Init();
    UniformManager::Get().Init();
    TextureManager::Get();
    ModelManager::Get();
    BufferManager::Get().Init();

    auto fuck = Model("Crab.gltf");
    auto anim = Animation("Crab.gltf", &fuck);
    auto animator = Animator(&anim);

    glm::vec3 pos = {0.0f, 0.0f, 0.0f};
    glm::vec3 orient = {1.0f, 0.0f, 0.0f};
    glm::vec3 up = {0.0f, 1.0f, 0.0f};

    glm::mat4 view{1.f};
    glm::mat4 proj{1.f};
    proj = glm::perspective(glm::radians(60.f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 50.f);
    view = glm::lookAt(pos, pos + orient, up);
    glm::mat4 projView = proj * view;

    glm::mat4 model{1.f};
    model = glm::translate(model, glm::vec3(8.f, -4.f, 0.f));
    model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.f, 1.f, 0.f));
    //model = glm::scale(model, glm::vec3(0.01));

    bool exit = false;
    SDL_Event event;
    while(!exit)
    {
        // Main input part of loop
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                exit = true;
                break;
            case SDL_WINDOWEVENT:
                const SDL_WindowEvent& winEv = event.window;
                switch (winEv.event)
                {
                case SDL_WINDOWEVENT_CLOSE:
                    exit = true;
                    break;
                }
                break;
            }
        }

        // Main logic part of loop
        animator.UpdateAnimation(0.016f);

        // "Advance" one frame (in terms of the rendering)
        // Main rendering part of loop
        bgfx::touch(0);

        // Model
        bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CW);

        bgfx::setUniform(UniformManager::Get().ProjView, &projView);
        bgfx::setUniform(UniformManager::Get().Model, &model);
        bgfx::setUniform(UniformManager::Get().Bones, animator.GetFinalBoneMatrices().data(), 50);

        for (auto& mesh : fuck.Meshes)
        {
            bgfx::setVertexBuffer(0, mesh.VBO);
            bgfx::setIndexBuffer(mesh.EBO);
            bgfx::setTexture(0, UniformManager::Get().Diffuse, mesh.Diffuse);
            bgfx::submit(0, ShaderManager::Get().SkinnedMesh);
        }

        bgfx::frame();  
    }

    // CLEAN UP ASSET MANAGERS
    ShaderManager::Get().Shutdown();
    UniformManager::Get().Shutdown();
    TextureManager::Get().Shutdown();
    ModelManager::Get().Shutdown();
    BufferManager::Get().Shutdown();

    // Clean up Window
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}