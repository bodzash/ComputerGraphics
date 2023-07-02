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
    GLFWwindow* window = glfwCreateWindow(800, 800, "Apex Legends", nullptr, nullptr);

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

    bgfx::ShaderHandle vsh = loadShader("Basic.vert.bin");
    bgfx::ShaderHandle fsh = loadShader("Basic.frag.bin");
    bgfx::ProgramHandle program = bgfx::createProgram(vsh, fsh, true);

    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Color;
        glm::vec3 Normal;
        glm::vec2 UV;
    };
    
    struct Mesh
    {
        std::vector<Vertex> Vertices;
        std::vector<uint16_t> Indicies;
        // ...
    };

    struct Model
    {
        std::vector<Mesh> meshes;
        // ...
    };

    std::vector<Vertex> vertices;
    std::vector<uint16_t> indicies;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    std::string file = "character-orc.obj";

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, file.c_str()))
    {
        throw std::runtime_error(warn + err);
    }

    for (auto& shape : shapes)
    {
        std::cout << shape.name << "\n";
    }

    uint32_t idx = 0;


    /*
    Model loading pseudo-code

    Model model

    for every shape in shapes
        Model add Mesh
        for every "mesh" in shape
            fill up Vertex with data
            Mesh add Vertex
            Mesh add 

    */

    for (const auto& index : shapes[5].mesh.indices)
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

        vertices.push_back(vertex);
        indicies.push_back(idx);
        idx++;
    }

    /*
    for (const auto& shape : shapes)
    {
        uint32_t idx = 0;

        for (const auto& index : shape.mesh.indices)
        {
            VertexData vertex;

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

            vertices.push_back(vertex);
            indicies.push_back(idx);
            idx++;
        }
    }
    */

    std::cout << indicies.size() << "\n";
    std::cout << vertices.size() << "\n";

    bgfx::VertexLayout vertexLayout;
    vertexLayout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .end();

    bgfx::VertexBufferHandle vertex_buffer = bgfx::createVertexBuffer(bgfx::makeRef(vertices.data(), sizeof(Vertex) * vertices.size()), vertexLayout);
    bgfx::IndexBufferHandle index_buffer = bgfx::createIndexBuffer(bgfx::makeRef(indicies.data(), sizeof(uint16_t) * indicies.size()));

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
    proj = glm::perspective(glm::radians(45.f), (float)(800/800), 0.1f, 100.f);

    bgfx::UniformHandle u_camMatrix = bgfx::createUniform("camMatrix", bgfx::UniformType::Mat4);

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

        //bgfx::setViewTransform(0, &view, &proj);
        //bgfx::setTransform(&model, 1);

        glm::mat4 view{1.f};
        glm::mat4 proj{1.f};

        view = glm::lookAt(pos, pos + orient, up);
        proj = glm::perspective(glm::radians(45.f), (float)(800/800), 0.01f, 100.f);

        auto lol = proj * view;
        //bgfx::setUniform(u_camMatrix, &lol, 1);

        // Cube
        glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.1f);

        bgfx::setVertexBuffer(0, vertex_buffer);
        bgfx::setIndexBuffer(index_buffer);
        bgfx::setUniform(u_lightcolor, &lightColor, 1);
        bgfx::setUniform(u_camMatrix, &lol, 1);
        bgfx::submit(0, program);

        bgfx::frame();
    }

    bgfx::shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
}