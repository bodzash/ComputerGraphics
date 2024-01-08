#include <iostream>
#include <vector>

#include <GLFW/glfw3.h>

#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include "bimg/bimg.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/rotate_vector.hpp>
#include <gtx/vector_angle.hpp>

#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

#include "ContentManagers/UniformManager.h"
#include "ContentManagers/ShaderManager.h"
//#include "ContentManagers/TextureManager.h"
#include "ContentManagers/BufferManager.h"
#include "Utility.h"

int main(int argc, char** argv)
{

    int WINDOW_WIDTH = 800;
    int WINDOW_HEIGHT = 600;

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Batch Renderer",
        nullptr, nullptr);

    bgfx::Init init;
    init.platformData.nwh = glfwGetWin32Window(window);
    init.type = bgfx::RendererType::Direct3D9;
    init.resolution.width = 800;
    init.resolution.height = 600;
    init.resolution.reset = 0 | BGFX_RESET_VSYNC | BGFX_RESET_FLUSH_AFTER_RENDER;

    // Init BGFX
    bgfx::init(init);

    // Resize framebuffers
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x8A8E8CFF, 1.0f);
	bgfx::setViewRect(0, 0, 0, 800, 600);

    // INIT ASSET MANAGERS, these shits should be remade
    ShaderManager::Get().Init();
    UniformManager::Get().Init();
    //TextureManager::Get();
    BufferManager::Get().Init();

    /*

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
    */

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // "Advance" one frame (in terms of the rendering)
        // Main rendering part of loop
        bgfx::touch(0);

        // Model
        bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CW);

        //bgfx::setUniform(UniformManager::Get().ProjView, &projView);
        //bgfx::setUniform(UniformManager::Get().Model, &model);
        //bgfx::setUniform(UniformManager::Get().Bones, animator.GetFinalBoneMatrices().data(), 50);

        /*
        bgfx::setVertexBuffer(0, mesh.VBO);
        bgfx::setIndexBuffer(mesh.EBO);
        bgfx::setTexture(0, UniformManager::Get().Diffuse, mesh.Diffuse);
        bgfx::submit(0, ShaderManager::Get().SkinnedMesh);
        */

        bgfx::frame();  
    }

    // CLEAN UP ASSET MANAGERS
    ShaderManager::Get().Shutdown();
    UniformManager::Get().Shutdown();
    //TextureManager::Get().Shutdown();
    BufferManager::Get().Shutdown();

    // Clean up window
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}