#include <iostream>
#include <GLFW/glfw3.h>
#include "bgfx/bgfx.h"
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

bgfx::ShaderHandle LoadShader(const char *FILENAME)
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

    FILE* f = fopen(filePath, "rb");
    fseek(f, 0, SEEK_END);
    const bgfx::Memory* mem = bgfx::alloc(ftell(f));
    fseek(f, 0, SEEK_SET);
    fread(mem->data, mem->size, 1, f);
    fclose(f);

    // CURSED: function doesnt seem to free memory >:(
    return bgfx::createShader(mem);
}

bgfx::ProgramHandle LoadShaderProgram(const std::string& filePath)
{
    bgfx::ShaderHandle vsh = LoadShader("Basic.vert.bin");
    bgfx::ShaderHandle fsh = LoadShader("Basic.frag.bin");
    return bgfx::createProgram(vsh, fsh, true);
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
    // ...
};

struct Material
{
    glm::vec4 Diffuse;
    glm::vec4 Specular;
    glm::vec4 Shininess;
};

struct Light
{
    glm::vec4 Position;
    glm::vec4 Ambient;
    glm::vec4 Diffuse;
    glm::vec4 Specular;
};

struct DirectionalLight
{
    glm::vec4 Direction;
    glm::vec4 Ambient;
    glm::vec4 Diffuse;
    glm::vec4 Specular;
};

struct PointLight
{
    glm::vec4 Position;  
  
    glm::vec4 Ambient;
    glm::vec4 Diffuse;
    glm::vec4 Specular;
	
    glm::vec4 Attenuation;
};

// IMPORTANT: IMAGE NEEDS TO BE FLIPPED VERTICALLY BEFORE LOADING
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
    texture->Handle = bgfx::createTexture(mem, BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE, 0, &textureInfo);
    //texture->Width = textureInfo.width;
    //texture->Height = textureInfo.height;

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
    int WINDOW_HEIGHT = 600;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Apex Legends", nullptr, nullptr);

    GLFWimage windowIcon;
    windowIcon.pixels = stbi_load("Icon.png", &windowIcon.width, &windowIcon.height, 0, 4);
    glfwSetWindowIcon(window, 1, &windowIcon); 
    stbi_image_free(windowIcon.pixels);

    bgfx::Init bgfxInit;
    bgfxInit.platformData.nwh = glfwGetWin32Window(window);
    bgfxInit.type = bgfx::RendererType::Direct3D9;
    bgfxInit.resolution.width = WINDOW_WIDTH;
    bgfxInit.resolution.height = WINDOW_HEIGHT;
    bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
    bgfx::init(bgfxInit);

    bgfx::ProgramHandle program = LoadShaderProgram("Basic");

    bgfx::VertexLayout vertexLayout;
    vertexLayout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .end();

    Mesh* mesh = LoadMeshObj("EliseSuperGalaxy.obj", vertexLayout);
    Texture* tex = LoadImageCompiled("EliseSuperGalaxy_Diff.dds");

    //Mesh* mesh = LoadMeshObj("Angel.obj", vertexLayout);
    //Texture* tex = LoadImageCompiled("Angel_Diff.dds");
    //Texture* texSpecular = LoadImageCompiled("Angel_Spec.dds");

    //Mesh* mesh = LoadMeshObj("viking_room.obj", vertexLayout);
    //Texture* tex = LoadImageCompiled("viking_room.dds");
    //Texture* texSpecular = LoadImageCompiled("viking_room_specular.dds");

    bgfx::UniformHandle u_texNormal = bgfx::createUniform("s_Albedo", bgfx::UniformType::Sampler);
    bgfx::UniformHandle u_texSpecular = bgfx::createUniform("s_Specular", bgfx::UniformType::Sampler);

    bgfx::UniformHandle u_model = bgfx::createUniform("u_Model", bgfx::UniformType::Mat4);
    bgfx::UniformHandle u_invmodel = bgfx::createUniform("u_InverseModel", bgfx::UniformType::Mat4);
    bgfx::UniformHandle u_viewposition = bgfx::createUniform("u_ViewPosition", bgfx::UniformType::Vec4);
    bgfx::UniformHandle u_material = bgfx::createUniform("u_Material", bgfx::UniformType::Vec4, 1);
    bgfx::UniformHandle u_light = bgfx::createUniform("u_Light", bgfx::UniformType::Vec4, 4);
    bgfx::UniformHandle u_plight = bgfx::createUniform("u_PointLight", bgfx::UniformType::Vec4, 5);

    glm::vec3 pos = {0.0f, 0.0f, 2.0f};
    glm::vec3 orient = {0.0f, 0.0f, -1.0f};
    glm::vec3 up = {0.0f, 1.0f, 0.0f};

    float speed = 2.f;
    float sens = 100.0f;
    bool firstClick = true;

    glm::mat4 view{1.f};
    glm::mat4 proj{1.f};

    bgfx::UniformHandle u_camMatrix = bgfx::createUniform("u_ProjView", bgfx::UniformType::Mat4);

    const bgfx::ViewId kClearView = 0;
	bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0); //0x443355FF //0x11212B
	bgfx::setViewRect(kClearView, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glm::vec4 lightPosition = glm::vec4(1.0f, 5.0f, 5.0f, 1.0f);

    Material materialData;
    materialData.Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    materialData.Specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    materialData.Shininess = glm::vec4(32.0f, 1.0f, 1.0f, 1.0f);

    Light lightData;
    lightData.Position = glm::vec4(-0.2f, -1.0f, -0.3f, 1.0f);
    //lightData.Position = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    lightData.Ambient = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
    lightData.Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    lightData.Specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    PointLight pLightData;
    pLightData.Position = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    pLightData.Ambient = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
    pLightData.Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    pLightData.Specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    pLightData.Attenuation = glm::vec4(1.0, 0.35, 0.44f, .0f);
    /*
    pLightData.Constant = 1.0f;
    pLightData.Linear = 0.09f;
    pLightData.Quadratic = 0.032f;
    */
    
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
            bgfx::reset(WINDOW_WIDTH, WINDOW_HEIGHT, BGFX_RESET_VSYNC);
	        bgfx::setViewRect(kClearView, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        }

        bgfx::touch(kClearView);
        bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CW);
        //bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CW | BGFX_STATE_MSAA);

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

        glm::mat4 model{1.f};
        glm::mat4 view{1.f};
        glm::mat4 proj{1.f};

        view = glm::lookAt(pos, pos + orient, up);
        proj = glm::perspective(glm::radians(63.f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 50.f);

        auto lol = proj * view;

        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            lightData.Position = glm::vec4(pos, 1.0f);

        if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
            pLightData.Position = glm::vec4(pos, 1.0f);

        if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            lightData.Position.y += 0.1;

        if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            lightData.Position.y -= 0.1;

        if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            lightData.Position.x += 0.1;

        if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            lightData.Position.x -= 0.1;
        
        glm::mat4 invmodel = glm::inverse(model);
        glm::vec4 viewPos(pos, 1.0f);

        bgfx::setUniform(u_camMatrix, &lol, 1);
        bgfx::setUniform(u_model, &model, 1);
        bgfx::setUniform(u_invmodel, &invmodel, 1);
        bgfx::setUniform(u_viewposition, &viewPos, 1);
        bgfx::setUniform(u_material, &materialData.Shininess, 1);
        //bgfx::setUniform(u_light, &lightData, 4);
        bgfx::setUniform(u_plight, &pLightData, 5);

        bgfx::setVertexBuffer(0, mesh->VertexBuffer);
        bgfx::setIndexBuffer(mesh->IndexBuffer);
        bgfx::setTexture(0, u_texNormal, tex->Handle);
        //bgfx::setTexture(1, u_texSpecular, texSpecular->Handle);
        bgfx::submit(0, program);

        model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
        invmodel = glm::inverse(model);

        bgfx::setUniform(u_model, &model, 1);
        bgfx::setUniform(u_invmodel, &invmodel, 1);

        bgfx::setVertexBuffer(0, mesh->VertexBuffer);
        bgfx::setIndexBuffer(mesh->IndexBuffer);
        bgfx::setTexture(0, u_texNormal, tex->Handle);
        //bgfx::setTexture(1, u_texSpecular, texSpecular->Handle);
        bgfx::submit(0, program);
        
        bgfx::frame();
    }

    bgfx::shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
}

/*

*/