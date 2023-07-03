//#define GLFW_INCLUDE_NONE
#include <iostream>
#include <GLFW/glfw3.h>
#include "bgfx/bgfx.h"
#include "bx/math.h"
#include "bgfx/platform.h"
#include "bimg/bimg.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/rotate_vector.hpp>
#include <gtx/vector_angle.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

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
        case bgfx::RendererType::Direct3D9:  shaderPath = "Shaders/DirectX9/";   break;
        case bgfx::RendererType::Direct3D11:
        case bgfx::RendererType::Direct3D12: shaderPath = "Shaders/DirectX11/";  break;
        case bgfx::RendererType::Gnm:        shaderPath = "Shaders/PSSL/";  break;
        case bgfx::RendererType::Metal:      shaderPath = "Shaders/Metal/"; break;
        case bgfx::RendererType::OpenGL:     shaderPath = "Shaders/GLSL/";  break;
        case bgfx::RendererType::OpenGLES:   shaderPath = "Shaders/ESSL/";  break;
        case bgfx::RendererType::Vulkan:     shaderPath = "Shaders/SPRIV/"; break;
    }

    size_t shaderLen = strlen(shaderPath);
    size_t fileLen = strlen(FILENAME);
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
    // CURSED: doesnt seem to free memory >:(
}

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Color;
    glm::vec3 Normal;
    glm::vec2 UV;
    // ...
};

struct Mesh
{
    std::vector<Vertex> Vertices;
    std::vector<uint16_t> Indicies;
    bgfx::VertexBufferHandle VertexBuffer;
    bgfx::IndexBufferHandle IndexBuffer;
    // ...
};

struct Model
{
    std::vector<Mesh> Meshes;
    // ...
};

struct Texture
{
    bgfx::TextureHandle Handle;
    uint16_t Width;
    uint16_t Height;
};

Texture* LoadImagePng(const std::string& filePath)
{
    Texture* texture = new Texture();

    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);

    unsigned char* image = stbi_load(filePath.c_str(), &width, &height, &channels, 0);

    const bgfx::Memory* textureMemory = bgfx::copy(image, width * height * channels);
    texture->Handle = bgfx::createTexture2D(width, height, false, 0, bgfx::TextureFormat::RGB8, 0, textureMemory);
    texture->Width = width;
    texture->Height = height;

    stbi_image_free(image);

    return texture;
}

Texture* LoadImageCompiled(const std::string& filePath)
{
    Texture* texture = new Texture();

    FILE* f = fopen(filePath.c_str(), "rb");
    fseek(f, 0, SEEK_END);
    const bgfx::Memory* mem = bgfx::alloc(ftell(f));
    fseek(f, 0, SEEK_SET);
    fread(mem->data, mem->size, 1, f);
    fclose(f);

    bgfx::TextureInfo textureInfo;
    texture->Handle = bgfx::createTexture(mem, BGFX_TEXTURE_NONE|BGFX_SAMPLER_NONE, 0, &textureInfo);
    texture->Width = textureInfo.width;
    texture->Height = textureInfo.height;


    std::cout << (int)textureInfo.numMips << "\n";
    std::cout << (int)textureInfo.numLayers << "\n";
    std::cout << (int)textureInfo.depth << "\n";
    std::cout << (int)textureInfo.width << "\n";
    std::cout << (int)textureInfo.height << "\n";
    std::cout << (int)textureInfo.bitsPerPixel << "\n";
    std::cout << (int)textureInfo.cubeMap << "\n";
    std::cout << (int)textureInfo.storageSize << "\n";
    std::cout << (int)textureInfo.format << "\n";


    return texture;
}


Mesh* LoadMeshObj(const std::string& filePath, bgfx::VertexLayout& vertexLayout)
{
    Mesh* mesh = new Mesh();

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filePath.c_str(), "./", true, true))
    {
        throw std::runtime_error(warn + err);
    }

    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            Vertex vertex;

            if (index.vertex_index >= 0)
            {
                vertex.Position = {
                    attrib.vertices[3 * index.vertex_index],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                auto colorIndex = 3 * index.vertex_index + 2;
                if (colorIndex < attrib.colors.size())
                {
                    vertex.Color = {
                        attrib.colors[colorIndex - 2],
                        attrib.colors[colorIndex - 1],
                        attrib.colors[colorIndex]
                    };
                }
                else
                {
                    vertex.Color = {1.0f, 1.0f, 1.0f};
                }
            }

            if (index.normal_index >= 0)
            {
                vertex.Normal = {
                    attrib.normals[3 * index.normal_index],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                };
            }

            if (index.texcoord_index >= 0)
            {
                vertex.UV = {
                    attrib.texcoords[2 * index.texcoord_index],
                    attrib.texcoords[2 * index.texcoord_index + 1]
                };
            }

            mesh->Vertices.push_back(vertex);
            mesh->Indicies.push_back(mesh->Indicies.size());
        }
    }

    mesh->VertexBuffer = bgfx::createVertexBuffer(bgfx::makeRef(mesh->Vertices.data(), sizeof(Vertex) * mesh->Vertices.size()), vertexLayout);
    mesh->IndexBuffer = bgfx::createIndexBuffer(bgfx::makeRef(mesh->Indicies.data(), sizeof(uint16_t) * mesh->Indicies.size()));

    return mesh;
}

int main(int argc, char **argv)
{
    glfwSetErrorCallback(glfw_errorCallback);
    glfwInit();

    int WINDOW_WIDTH = 800;
    int WINDOW_HEIGHT = 800;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Apex Legends", nullptr, nullptr);

    bgfx::Init bgfxInit;
    bgfxInit.platformData.nwh = glfwGetWin32Window(window);
    bgfxInit.type = bgfx::RendererType::Direct3D9;
    bgfxInit.resolution.width = WINDOW_WIDTH;
    bgfxInit.resolution.height = WINDOW_HEIGHT;
    bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
        
    bgfx::init(bgfxInit);

    // Create a view and set it to the same dimensions as the window
    const bgfx::ViewId kClearView = 0;
	bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x11212B); //0x443355FF
	bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);

    bgfx::ShaderHandle vsh = loadShader("Basic.vert.bin");
    bgfx::ShaderHandle fsh = loadShader("Basic.frag.bin");
    bgfx::ProgramHandle program = bgfx::createProgram(vsh, fsh, true);

    bgfx::VertexLayout vertexLayout;
    vertexLayout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .end();

    Mesh* mesh = LoadMeshObj("viking_room.obj", vertexLayout);
    //Texture* tex = LoadImageCompiled("viking_room.dds");
    Texture* tex = LoadImagePng("viking_room.png");

    bgfx::UniformHandle u_texNormal = bgfx::createUniform("u_texNormal", bgfx::UniformType::Sampler);

    bgfx::UniformHandle u_model = bgfx::createUniform("model", bgfx::UniformType::Mat4);
    bgfx::UniformHandle u_lightcolor = bgfx::createUniform("lightColor", bgfx::UniformType::Vec4);

    glm::vec3 pos = {0.0f, 0.0f,  2.0f};
    glm::vec3 orient = {0.0f, 0.0f,  -1.0f};
    glm::vec3 up = {0.0f, 1.0f,  0.0f};

    float speed = 10.f;
    float sens = 100.0f;
    bool firstClick = true;

    glm::mat4 view{1.f};
    glm::mat4 proj{1.f};

    view = glm::lookAt(pos, pos + orient, up);
    proj = glm::perspective(glm::radians(45.f), (float)(WINDOW_WIDTH/WINDOW_HEIGHT), 0.1f, 100.f);

    bgfx::UniformHandle u_camMatrix = bgfx::createUniform("camMatrix", bgfx::UniformType::Mat4);

    while(!glfwWindowShouldClose(window))
    {
        // Polls events
        glfwPollEvents();

        /*
        std::int32_t display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        bgfx::reset(display_w, display_h, BGFX_RESET_VSYNC);
        bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
        */

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
                glfwSetCursorPos(window, (WINDOW_WIDTH / 2), (WINDOW_HEIGHT / 2));
                firstClick = false;
            }

            double x, y;
            glfwGetCursorPos(window, &x, &y);

            float rotx = sens * (float)(y - (WINDOW_HEIGHT / 2)) / WINDOW_HEIGHT; // width
            float roty = sens * (float)(x - (WINDOW_WIDTH / 2)) / WINDOW_HEIGHT; // height

            glm::vec3 newOrientation = glm::rotate(orient, glm::radians(-rotx), glm::normalize(glm::cross(orient, up)));

            // Decides whether or not the next vertical Orientation is legal or not
            if (abs(glm::angle(newOrientation, up) - glm::radians(90.0f)) <= glm::radians(85.0f))
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

        glm::mat4 view{1.f};
        glm::mat4 proj{1.f};

        view = glm::lookAt(pos, pos + orient, up);
        view = glm::rotate(view, glm::radians(90.f), glm::vec3(-1, 0, 0));
        proj = glm::perspective(glm::radians(45.f), (float)(WINDOW_WIDTH/WINDOW_HEIGHT), 0.01f, 100.f);

        auto lol = proj * view;

        glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.1f);

        bgfx::setVertexBuffer(0, mesh->VertexBuffer);
        bgfx::setIndexBuffer(mesh->IndexBuffer);
        bgfx::setTexture(0, u_texNormal, tex->Handle);
        bgfx::setUniform(u_lightcolor, &lightColor, 1);
        bgfx::setUniform(u_camMatrix, &lol, 1);
        bgfx::submit(0, program);
        
        bgfx::frame();
    }

    bgfx::shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
}