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

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

// Let the "fun" begin
#include "Renderer.h"
#include "Model.h"
#include "Level.h"
#include "PlayerActor.h"
#include "ContentManagers/UniformManager.h"
#include "ContentManagers/ShaderManager.h"
#include "ContentManagers/TextureManager.h"
#include "ContentManagers/BufferManager.h"
#include "ContentManagers/ModelManager.h"
#include "Utility.h"

#define clog(x) std::cout << x << std::endl

static void OnGLFWError(int error, const char *description)
{
	fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

int main(int argc, char** argv)
{
    glfwSetErrorCallback(OnGLFWError);
    glfwInit();

    int WINDOW_WIDTH = 800;
    int WINDOW_HEIGHT = 600;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Eternite", nullptr, nullptr);

    /*
    //clog(bgfx::getCaps()->limits.maxDrawCalls);
    clog(bgfx::getCaps()->homogeneousDepth);
    clog(bgfx::getCaps()->limits.maxFBAttachments);
    clog(bgfx::getCaps()->limits.maxOcclusionQueries);
    std::cout << (bool)(bgfx::getCaps()->supported & BGFX_CAPS_RENDERER_MULTITHREADED) << '\n';
    std::cout << (bool)(bgfx::getCaps()->supported & BGFX_CAPS_OCCLUSION_QUERY) << '\n';
    std::cout << (bool)(bgfx::getCaps()->supported & BGFX_CAPS_TEXTURE_BLIT) << '\n';
    std::cout << (bool)(bgfx::getCaps()->supported & BGFX_CAPS_SWAP_CHAIN) << '\n';
    std::cout << (bool)(bgfx::getCaps()->supported & BGFX_CAPS_COMPUTE) << '\n';
    */

    Level m_Level;
    auto player = m_Level.CreateActor<PlayerActor>();

    Renderer m_Renderer(m_Level);
    m_Renderer.Init(glfwGetWin32Window(window));

    // INIT ASSET MANAGERS
    ShaderManager::Get().Init();
    UniformManager::Get().Init();
    TextureManager::Get();
    ModelManager::Get();
    BufferManager::Get().Init();

    //auto& angel = ModelManager::Get().LoadSkinned("Content/Models/LeBlanc/Leblanc_Skin04.dae");
    //auto& angel = ModelManager::Get().LoadSkinned("Content/Models/model.dae");
    //auto fuck = Model("Content/Models/LeBlanc/Leblanc_Skin04.dae");
    //auto& angel = ModelManager::Get().LoadSkinned("Content/Models/Angel/Skel_VoG.dae");   

    //auto skyboxTexture = TextureManager::Get().Load("Content/Textures/Skyboxes/SkyboxDay.dds");

    auto fuck = Model("Content/Models/model.dae");
    auto anim = Animation("Content/Models/model.dae", &fuck);
    //auto fuck = Model("Content/Models/Vampire/dancing_vampire.dae");
    //auto anim = Animation("Content/Models/Vampire/dancing_vampire.dae", &fuck);
    auto animator = Animator(&anim);

    glm::vec3 pos = {0.0f, 0.0f, 0.0f};
    glm::vec3 orient = {1.0f, 0.0f, 0.0f};
    glm::vec3 up = {0.0f, 1.0f, 0.0f};

    float speed = 2.f;
    float sens = 100.0f;
    bool firstClick = true;

	//bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f); //0x443355FF //0x11212B // 0x8A8E8C
	//bgfx::setViewRect(0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    glm::mat4 view{1.f};
    glm::mat4 proj{1.f};
    proj = glm::perspective(glm::radians(60.f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 50.f);
    //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));

    glm::mat4 model{1.f};
    //model = glm::scale(model, glm::vec3(0.01));

    glm::mat4 bone{1.0f};
    bone = glm::rotate(bone, glm::radians(45.0f), glm::vec3(45.f, 0.0f, 0.0f));
 
    while(!glfwWindowShouldClose(window))
    {
        // Polls events
        glfwPollEvents();
        
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        if (width != WINDOW_WIDTH || height != WINDOW_HEIGHT)
        {
            WINDOW_WIDTH = width;
            WINDOW_HEIGHT = height;
            bgfx::reset(WINDOW_WIDTH, WINDOW_HEIGHT, BGFX_RESET_VSYNC); // BGFX_RESET_SRGB_BACKBUFFER
            bgfx::setViewRect(0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        }
        

#pragma region Controls

        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            break;

        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            pos += speed * orient;

        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            pos += speed * -glm::normalize(glm::cross(orient, up));

        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            pos += speed * -orient;

        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            pos += speed * glm::normalize(glm::cross(orient, up));

        if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            pos += speed * up;

        if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            pos += speed * -up;

        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            speed = 0.01f;

        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
            speed = 0.1f;

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

            if (firstClick)
            {
                glfwSetCursorPos(window, (WINDOW_WIDTH / 2), (WINDOW_HEIGHT / 2));
                firstClick = false;
            }

            double x, y;
            glfwGetCursorPos(window, &x, &y);

            float rotx = sens * (float)(y - (WINDOW_HEIGHT / 2)) / WINDOW_HEIGHT;
            float roty = sens * (float)(x - (WINDOW_WIDTH / 2)) / WINDOW_HEIGHT;

            glm::vec3 newOrientation = glm::rotate(orient, glm::radians(-rotx), glm::normalize(glm::cross(orient, up)));

            // Decides whether or not the next vertical Orientation is legal or not
            if (abs(glm::angle(newOrientation, up) - glm::radians(89.0f)) <= glm::radians(89.0f))
                orient = newOrientation;

            // Rotates the Orientation left and right
            orient = glm::rotate(orient, glm::radians(-roty), up);

            // Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
            glfwSetCursorPos(window, (WINDOW_WIDTH / 2), (WINDOW_HEIGHT / 2));
        }
        else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            firstClick = true;
        }

#pragma endregion

        view = glm::lookAt(pos, pos + orient, up);
        
        glm::mat4 projView = proj * view;

        bgfx::touch(0);

        animator.UpdateAnimation(0.016f);

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

        /*
        for (auto& mesh : angel.Meshes)
        {
            // Bind buffers
            bgfx::setVertexBuffer(0, mesh.VBO);
            bgfx::setIndexBuffer(mesh.EBO);

            // Bind textures
            bgfx::setTexture(0, UniformManager::Get().Diffuse, mesh.Diffuse);

            // Submit
            bgfx::submit(0, ShaderManager::Get().SkinnedMesh);
            //bgfx::submit(0, ShaderManager::Get().StaticMesh);
        }
        */

        bgfx::frame();  
    }

    // CLEAN UP ASSET MANAGERS
    ShaderManager::Get().Shutdown();
    UniformManager::Get().Shutdown();
    TextureManager::Get().Shutdown();
    ModelManager::Get().Shutdown();
    BufferManager::Get().Shutdown();
    
    // Clean up Renderer
    m_Renderer.Shutdown();

    // Clean up Window
    glfwDestroyWindow(window);
    glfwTerminate();
}