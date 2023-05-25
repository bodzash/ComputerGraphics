//#define GLFW_INCLUDE_NONE
#include <iostream>
#include <GLFW/glfw3.h>
#include "bgfx/bgfx.h"
#include "bx/math.h"
#include "bgfx/platform.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/rotate_vector.hpp>
#include <gtx/vector_angle.hpp>
//#include "common.hpp"
#include "stb_image.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

static void glfw_errorCallback(int error, const char *description)
{
	fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

bgfx::ShaderHandle loadShader(const char *FILENAME)
{
    const char* shaderPath = "???";

    switch(bgfx::getRendererType()) {
        case bgfx::RendererType::Noop:
        case bgfx::RendererType::Direct3D9:  shaderPath = "shaders/dx9/";   break;
        case bgfx::RendererType::Direct3D11:
        case bgfx::RendererType::Direct3D12: shaderPath = "shaders/dx11/";  break;
        case bgfx::RendererType::Gnm:        shaderPath = "shaders/pssl/";  break;
        case bgfx::RendererType::Metal:      shaderPath = "shaders/metal/"; break;
        case bgfx::RendererType::OpenGL:     shaderPath = "shaders/glsl/";  break;
        case bgfx::RendererType::OpenGLES:   shaderPath = "shaders/essl/";  break;
        case bgfx::RendererType::Vulkan:     shaderPath = "shaders/spirv/"; break;
    }

    size_t shaderLen = strlen(shaderPath);
    size_t fileLen = strlen(FILENAME);
    //char *filePath = (char *)malloc(shaderLen + fileLen);
    char* filePath = (char*)calloc(1, shaderLen + fileLen + 1);
    memcpy(filePath, shaderPath, shaderLen);
    memcpy(&filePath[shaderLen], FILENAME, fileLen);

    FILE *file = fopen(filePath, "rb");
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    const bgfx::Memory *mem = bgfx::alloc(fileSize + 1);
    fread(mem->data, 1, fileSize, file);
    mem->data[mem->size - 1] = '\0';
    fclose(file);

    return bgfx::createShader(mem);
}

int main(int argc, char **argv)
{
    glfwSetErrorCallback(glfw_errorCallback);
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(800, 800, "Hello, bgfx!", nullptr, nullptr);

    bgfx::Init bgfxInit;
    bgfxInit.platformData.nwh = glfwGetWin32Window(window);
    bgfxInit.type = bgfx::RendererType::Count;
    bgfxInit.resolution.width = 800;
    bgfxInit.resolution.height = 800;
    bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
    
    bgfx::init(bgfxInit);

    // Create a view and set it to the same dimensions as the window
    const bgfx::ViewId kClearView = 0;
	bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x11212B); //0x443355FF
	bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);

    bgfx::ShaderHandle vsh = loadShader("vs_basic.bin");
    bgfx::ShaderHandle fsh = loadShader("fs_basic.bin");
    bgfx::ProgramHandle program = bgfx::createProgram(vsh, fsh, true);

    bgfx::VertexLayout vertexLayout;
    vertexLayout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .end();
    
    float vertices[] =
    {
        -0.5f,  0.5f,  0.5f,    1.f, 0.f, 1.f,      0, 0,
         0.5f,  0.5f,  0.5f,    0.f, 1.0f, 1.0f,    0, 1,
        -0.5f, -0.5f,  0.5f,    0.f, 1.f, 0.f,      1, 0,
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f, 0.0f,   1, 1,
        -0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 1.0f,   0, 0,
         0.5f,  0.5f, -0.5f,    1.0f, 1.f, 0.f,     1, 0,
        -0.5f, -0.5f, -0.5f,    1.0f, 1.f, 1.f,     0, 1,
         0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 0.0f,   1, 1
    };

    /*
    float vertices[] =
    {
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
    };
    */


    const uint16_t indicies[] =
    {
        0, 1, 2,
        1, 3, 2,
        4, 6, 5,
        5, 6, 7,
        0, 2, 4,
        4, 2, 6,
        1, 5, 3,
        5, 7, 3,
        0, 4, 1,
        4, 5, 1,
        2, 3, 6,
        6, 3, 7
    };

    /*
    glm::mat4 model{1.f};
    glm::mat4 view{1.f};
    glm::mat4 proj{1.f};
    view = glm::translate(view, glm::vec3(0.f, -0.5f, -2.0f));
    proj = glm::perspective(glm::radians(45.0f), float(800/800), 0.1f, 100.f);

    bgfx::UniformHandle u_model = bgfx::createUniform("model", bgfx::UniformType::Mat4);
    bgfx::setUniform(u_model, &model, 1);

    bgfx::UniformHandle u_view = bgfx::createUniform("view", bgfx::UniformType::Mat4);
    bgfx::setUniform(u_view, &view, 1);

    bgfx::UniformHandle u_proj = bgfx::createUniform("proj", bgfx::UniformType::Mat4);
    bgfx::setUniform(u_proj, &proj, 1);

    */

    bgfx::VertexBufferHandle vertex_buffer = bgfx::createVertexBuffer(bgfx::makeRef(vertices, sizeof(vertices)), vertexLayout);
    bgfx::IndexBufferHandle index_buffer = bgfx::createIndexBuffer(bgfx::makeRef(indicies, sizeof(indicies)));

    int width, height, colch;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load("wall.png", &width, &height, &colch, 0);
    //unsigned char* bytes = stbi_load("tex.png", &width, &height, &colch, 0);

    std::cout << colch;


    bgfx::UniformHandle u_texNormal = bgfx::createUniform("u_texNormal", bgfx::UniformType::Sampler);

    const bgfx::Memory* textureMem = bgfx::copy(bytes, width * height * colch);
    bgfx::TextureHandle texture = bgfx::createTexture2D(width, height, false, 0, bgfx::TextureFormat::RGB8, 0, textureMem);

    // CURSED free raw loaded texture after we created a texture in bgfx
    free(bytes);

    glm::vec3 pos = {0.0f, 0.0f,  2.0f};
    glm::vec3 orient = {0.0f, 0.0f,  -1.0f};
    glm::vec3 up = {0.0f, 1.0f,  0.0f};
    float speed = 0.1f;
    float sens = 100.0f;
    bool firstClick = true;

    glm::mat4 view{1.f};
    glm::mat4 proj{1.f};

    view = glm::lookAt(pos, pos + orient, up);
    proj = glm::perspective(glm::radians(45.f), (float)(800/800), 0.1f, 100.f);

    bgfx::UniformHandle u_camMatrix = bgfx::createUniform("camMatrix", bgfx::UniformType::Mat4);
    //bgfx::setUniform(u_camMatrix, &(proj * view), 1);
    

    while(!glfwWindowShouldClose(window))
    {
        // Polls events
        glfwPollEvents();

        std::int32_t display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        bgfx::reset(display_w, display_h, BGFX_RESET_VSYNC);
        bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);

        // This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.
        bgfx::touch(kClearView);

        bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS);

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
                glfwSetCursorPos(window, (800 / 2), (800 / 2));
                firstClick = false;
            }

            double x, y;
            glfwGetCursorPos(window, &x, &y);

            float rotx = sens * (float)(y - (800 / 2)) / 800; // width
            float roty = sens * (float)(x - (800 / 2)) / 800; // height

            glm::vec3 newOrientation = glm::rotate(orient, glm::radians(-rotx), glm::normalize(glm::cross(orient, up)));

            // Decides whether or not the next vertical Orientation is legal or not
            if (abs(glm::angle(newOrientation, up) - glm::radians(90.0f)) <= glm::radians(85.0f))
                orient = newOrientation;

            // Rotates the Orientation left and right
            orient = glm::rotate(orient, glm::radians(-roty), up);

            // Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
            glfwSetCursorPos(window, (800 / 2), (800 / 2));
        }

        else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            firstClick = true;
        }

        /*
        
        const bx::Vec3 at = {0.0f, 0.0f,  0.0f};
        const bx::Vec3 eye = {0.0f, 0.0f, -5.0f};

        float view[16];
        bx::mtxLookAt(view, eye, at);
        float proj[16];
        bx::mtxProj(proj, 60.0f, float(800) / float(800), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
        */

        //bgfx::setViewTransform(0, &view, &proj);
        //bgfx::setTransform(&model, 1);

        glm::mat4 view{1.f};
        glm::mat4 proj{1.f};

        view = glm::lookAt(pos, pos + orient, up);
        proj = glm::perspective(glm::radians(45.f), (float)(800/800), 0.01f, 100.f);

        auto lol = proj * view;
        bgfx::setUniform(u_camMatrix, &lol, 1);

        bgfx::setVertexBuffer(0, vertex_buffer);
        bgfx::setIndexBuffer(index_buffer); // not needed if you don't do indexed draws

        bgfx::setTexture(0, u_texNormal, texture);

        bgfx::submit(0, program);

        bgfx::frame();
    }

    bgfx::shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
}