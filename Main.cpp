//#define GLFW_INCLUDE_NONE
#include <iostream>
#include <GLFW/glfw3.h>
#include "bgfx/bgfx.h"
#include "bx/math.h"
#include "bgfx/platform.h"
#include "glm.hpp"
#include "Shader.h"
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
	bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR, 0x11212B); //0x443355FF
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
        -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner
        -0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner
        0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper right corner
        0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f  // Lower right corner
    };


    const uint16_t indicies[] =
    {
        0, 2, 1, // Upper triangle
        0, 3, 2 // Lower triangle
    };

    bgfx::UniformHandle u_scale = bgfx::createUniform("u_scale", bgfx::UniformType::Vec4);

    float scale = 1.0f;
    bgfx::setUniform(u_scale, &scale, 1);

    bgfx::VertexBufferHandle vertex_buffer = bgfx::createVertexBuffer(bgfx::makeRef(vertices, sizeof(vertices)), vertexLayout);
    bgfx::IndexBufferHandle index_buffer = bgfx::createIndexBuffer(bgfx::makeRef(indicies, sizeof(indicies)));

    int width, height, colch;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load("tex.png", &width, &height, &colch, 0);

    bgfx::UniformHandle u_texNormal = bgfx::createUniform("u_texNormal", bgfx::UniformType::Sampler);

    const bgfx::Memory* textureMem = bgfx::copy(bytes, width * height * colch);
    bgfx::TextureHandle texture = bgfx::createTexture2D(width, height, false, 0, bgfx::TextureFormat::RGB8, 0, textureMem);

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

        bgfx::setState(
            BGFX_STATE_WRITE_R | BGFX_STATE_WRITE_G | BGFX_STATE_WRITE_B | BGFX_STATE_WRITE_A
        );

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