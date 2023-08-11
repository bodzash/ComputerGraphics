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

// This is some stupid shit
struct Material
{
    glm::vec4 Diffuse;
    glm::vec4 Specular;
    glm::vec4 Normal;
    glm::vec4 Emission;
    glm::vec4 Shininess;
};

struct DirectionalLight
{
    glm::vec4 Direction;
    glm::vec4 Ambient;
    glm::vec4 Diffuse;
    glm::vec4 Specular;
};

/*
struct DirectionalLight
{
    glm::vec3 Direction;
    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 Specular;
};
*/

int main(int argc, char** argv)
{
    glfwSetErrorCallback(OnGLFWError);
    glfwInit();

    int WINDOW_WIDTH = 800;
    int WINDOW_HEIGHT = 600;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Renderer", nullptr, nullptr);

    Renderer::Get().Init(glfwGetWin32Window(window));
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

    // INIT ASSET MANAGERS
    ShaderManager::Get().Init();
    UniformManager::Get().Init();
    TextureManager::Get();
    ModelManager::Get();
    BufferManager::Get().Init();

    Level level;
    auto player = level.CreateActor<PlayerActor>();

#pragma region VertexData

    float quadVerticesData[] = {
        // Position          // Text coords (V/Y FLIPPED!!!!!)
        -0.5f,  0.5f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.0f,  1.0f,  1.0f,

        -0.5f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.0f,  1.0f,  0.0f
    };

    uint16_t quadIndicesData[] = {
        0, 1, 2, 3, 4, 5
    };

    float frameVerticesData[] = {
        // Position   // Text coords (V OR Y FLIPPED!!!!!)
        -1.0f,  1.0f,  0.0f,  0.0f,
        -1.0f, -1.0f,  0.0f,  1.0f,
        1.0f, -1.0f,  1.0f,  1.0f,

        -1.0f,  1.f,  0.0f,  0.0f,
        1.0f, -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,  0.0f
    };

    uint16_t frameIndicesData[] = {
        0, 1, 2, 3, 4, 5
    };

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    uint16_t skyboxIndiciesData[] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
        10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
        20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
        30, 31, 32, 33, 34, 35
    };
    
#pragma endregion

#pragma region VertexLayout
    bgfx::VertexLayout quadVertexLayout;
    quadVertexLayout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .end();

    bgfx::VertexBufferHandle qvbo = bgfx::createVertexBuffer(bgfx::makeRef(quadVerticesData, sizeof(float) * 30), quadVertexLayout);
    bgfx::IndexBufferHandle qebo = bgfx::createIndexBuffer(bgfx::makeRef(quadIndicesData, sizeof(uint16_t) * 6));

    bgfx::VertexLayout skyboxVertexLayout;
    skyboxVertexLayout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .end();
    
    bgfx::VertexBufferHandle svbo = bgfx::createVertexBuffer(bgfx::makeRef(skyboxVertices, sizeof(float) * 108), skyboxVertexLayout);
    //bgfx::VertexBufferHandle svbo = bgfx::createVertexBuffer(bgfx::makeRef(skyboxVertices, sizeof(float) * 108), Skybox::GetLayout());
    bgfx::IndexBufferHandle sebo = bgfx::createIndexBuffer(bgfx::makeRef(skyboxIndiciesData, sizeof(uint16_t) * 36));

    bgfx::VertexLayout frameVertexLayout;
    frameVertexLayout.begin()
        .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .end();

    bgfx::VertexBufferHandle fvbo = bgfx::createVertexBuffer(bgfx::makeRef(frameVerticesData, sizeof(float) * 24), frameVertexLayout);
    bgfx::IndexBufferHandle febo = bgfx::createIndexBuffer(bgfx::makeRef(frameIndicesData, sizeof(uint16_t) * 6));

    bgfx::VertexLayout staticVertexLayout;
    staticVertexLayout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        // tang
        // bitang
        .end();

    #define MAX_BONE_INFULENCE 4
    bgfx::VertexLayout animatedVertexLayout;
    animatedVertexLayout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Tangent, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Bitangent, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord1, MAX_BONE_INFULENCE, bgfx::AttribType::Int16) // bone id
        .add(bgfx::Attrib::Weight, MAX_BONE_INFULENCE, bgfx::AttribType::Float) // weight
        .end();

#pragma endregion

    //Model mdl("Content/Models/Cube/Cube.fbx", staticVertexLayout);
    //Model mdl("Content/Models/phoenixSimple.blend", staticVertexLayout);
    //Model mdl("Content/Models/Jack/HandsomeJack.dae", staticVertexLayout);
    //Model mdl("Content/Models/Vampire/dancing_vampire.dae", staticVertexLayout);
    //Model mdl("Content/Models/Angel/Skel_VoG.dae", staticVertexLayout);

    auto& angel = ModelManager::Get().LoadStatic("Content/Models/Angel/Skel_VoG.dae");

    auto skyboxTexture = TextureManager::Get().Load("Content/Textures/Skyboxes/SkyboxDay.dds");

    glm::vec3 pos = {0.0f, 0.0f, 0.0f};
    glm::vec3 orient = {1.0f, 0.0f, 0.0f};
    glm::vec3 up = {0.0f, 1.0f, 0.0f};

    float speed = 2.f;
    float sens = 100.0f;
    bool firstClick = true;

	//bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f); //0x443355FF //0x11212B
	//bgfx::setViewRect(0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    Material materialData;
    materialData.Shininess = glm::vec4(32.0f);

    DirectionalLight dlightData;
    dlightData.Direction = glm::vec4(-0.2f, -1.0f, -0.3f, 1.0f); 
    dlightData.Ambient = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    dlightData.Diffuse = glm::vec4(0.8f);
    dlightData.Specular = glm::vec4(0.4f);
    
    //glm::mat4 model{1.f};
    glm::mat4 view{1.f};
    glm::mat4 proj{1.f};
    proj = glm::perspective(glm::radians(60.f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 50.f);
    //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));

    glm::mat4 model{1.f};
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
    
    while(!glfwWindowShouldClose(window))
    {
        // Polls events
        glfwPollEvents();
        level.OnUpdate();
        level.OnRender();

        
        /*
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        if (width != WINDOW_WIDTH || height != WINDOW_HEIGHT)
        {
            WINDOW_WIDTH = width;
            WINDOW_HEIGHT = height;
            bgfx::reset(WINDOW_WIDTH, WINDOW_HEIGHT, BGFX_RESET_VSYNC); // BGFX_RESET_SRGB_BACKBUFFER
            bgfx::setViewRect(0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        }
        */

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
        
        auto lol = proj * view;

        // Update
        level.OnUpdate();

        // Model
        bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CW);

        glm::mat4 invmodel = glm::inverse(model);
        glm::vec4 viewPos(pos, 1.0f);
        bgfx::setUniform(UniformManager::Get().ProjView, &lol);
        bgfx::setUniform(UniformManager::Get().Model, &model);
        bgfx::setUniform(UniformManager::Get().InverseModel, &invmodel);
        bgfx::setUniform(UniformManager::Get().Material, &materialData.Shininess);
        bgfx::setUniform(UniformManager::Get().DirLight, &dlightData, 4);
        bgfx::setUniform(UniformManager::Get().ViewPosition, &viewPos);

        for (auto& mesh : angel.Meshes)
        {
            bgfx::setVertexBuffer(0, mesh.VBO);
            bgfx::setIndexBuffer(mesh.EBO);

            // Bind textures
            bgfx::setTexture(0, UniformManager::Get().Diffuse, mesh.Diffuse);
            bgfx::setTexture(1, UniformManager::Get().Specular, mesh.Specular);

            // Submit
            bgfx::submit(0, ShaderManager::Get().StaticMesh);
        }    
    }

    // CLEAN UP ASSET MANAGERS
    ShaderManager::Get().Shutdown();
    UniformManager::Get().Shutdown();
    TextureManager::Get().Shutdown();
    ModelManager::Get().Shutdown();
    BufferManager::Get().Shutdown();
    
    // Clean up Renderer
    Renderer::Get().Shutdown();

    // Clean up Window
    glfwDestroyWindow(window);
    glfwTerminate();
}