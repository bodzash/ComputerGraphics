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

    struct VertexData
    {
        float x, y, z;
        //float r, g, b;
    };

    std::string inputfile = "building_cabin.obj";
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./"; // Path to material files
    reader_config.vertex_color = true;
    reader_config.triangulate = true;

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(inputfile, reader_config)) {
    if (!reader.Error().empty()) {
        std::cerr << "TinyObjReader: " << reader.Error();
    }
    
    }

    if (!reader.Warning().empty()) {
    std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    std::vector<float> vertices;
    std::vector<uint16_t> indicies;

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
    // Loop over faces(polygon)
    size_t index_offset = 0;
    for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
        size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

        // Loop over vertices in the face.
        for (size_t v = 0; v < fv; v++) {
        // access to vertex
        tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

        tinyobj::real_t vx = attrib.vertices[3*size_t(idx.vertex_index)+0];
        tinyobj::real_t vy = attrib.vertices[3*size_t(idx.vertex_index)+1];
        tinyobj::real_t vz = attrib.vertices[3*size_t(idx.vertex_index)+2];

        // Check if `normal_index` is zero or positive. negative = no normal data
        if (idx.normal_index >= 0) {
            tinyobj::real_t nx = attrib.normals[3*size_t(idx.normal_index)+0];
            tinyobj::real_t ny = attrib.normals[3*size_t(idx.normal_index)+1];
            tinyobj::real_t nz = attrib.normals[3*size_t(idx.normal_index)+2];
        }

        // Check if `texcoord_index` is zero or positive. negative = no texcoord data
        if (idx.texcoord_index >= 0) {
            tinyobj::real_t tx = attrib.texcoords[2*size_t(idx.texcoord_index)+0];
            tinyobj::real_t ty = attrib.texcoords[2*size_t(idx.texcoord_index)+1];
        }

        // Optional: vertex colors
        tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
        tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
        tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];

        //std::cout << "R: " << red << " B: " << green << " B:" << blue << '\n';

        vertices.push_back(attrib.vertices[3 * size_t(idx.vertex_index)+0]);
        vertices.push_back(attrib.vertices[3 * size_t(idx.vertex_index)+1]);
        vertices.push_back(attrib.vertices[3 * size_t(idx.vertex_index)+2]);

        vertices.emplace_back(red);
        vertices.emplace_back(green);
        vertices.emplace_back(blue);

        indicies.emplace_back(idx.vertex_index);

        }
        index_offset += fv;

        // per-face material
    }
    }

    //std::vector<float> vertices(attrib.vertices);

    //for (int i )

    bgfx::VertexLayout vertexLayout;
    vertexLayout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 3, bgfx::AttribType::Float)
        //.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .end();
    
    for (int i = 0; i < 30; i += 3)
    {
        std::cout << "X: " << vertices[i] << " Y: " <<  vertices[i + 1] << " Z: " << vertices[i + 2] << '\n';
    }

    std::cout << "-------\n";

    for (int i = 0; i < 30; i += 3)
    {
        std::cout << "X: " << attrib.vertices[i] << " Y: " << attrib.vertices[i +1] << " Z: " << attrib.vertices[i +2] << '\n';
    }

    /*
    for (int i = 0; i < 3; i++)
    {
        std::cout << indicies[i] << '\n';
    }
    */

    bgfx::VertexBufferHandle vertex_buffer = bgfx::createVertexBuffer(bgfx::makeRef(vertices.data(), sizeof(float) * vertices.size()), vertexLayout);
    bgfx::IndexBufferHandle index_buffer = bgfx::createIndexBuffer(bgfx::makeRef(indicies.data(), sizeof(uint16_t) * indicies.size()));

    //bgfx::VertexBufferHandle vertex_buffer = bgfx::createVertexBuffer(bgfx::makeRef(attrib.vertices.data(), sizeof(float) * 9), vertexLayout);
    //bgfx::IndexBufferHandle index_buffer = bgfx::createIndexBuffer(bgfx::makeRef(indicies.data(), sizeof(uint16_t) * 3));

    //bgfx::VertexBufferHandle vertex_buffer = bgfx::createVertexBuffer(bgfx::makeRef(attrib.vertices.data(), sizeof(attrib.vertices.data())), vertexLayout);
    //bgfx::IndexBufferHandle index_buffer = bgfx::createIndexBuffer(bgfx::makeRef(indicies.data(), sizeof(indicies.data())));

    //bgfx::VertexBufferHandle vertex_buffer = bgfx::createVertexBuffer(bgfx::makeRef(vertices, sizeof(vertices)), vertexLayout);
    //bgfx::IndexBufferHandle index_buffer = bgfx::createIndexBuffer(bgfx::makeRef(indicies, sizeof(indicies)));

    bgfx::UniformHandle u_model = bgfx::createUniform("model", bgfx::UniformType::Mat4);
    bgfx::UniformHandle u_lightcolor = bgfx::createUniform("lightColor", bgfx::UniformType::Vec4);

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

/*
// Submit 11x11 cubes. NOTE: these are seperate render submissions
// For instancing look here: https://github.com/bkaradzic/bgfx/blob/master/examples/05-instancing/instancing.cpp#LL238C4-L238C4
for (uint32_t yy = 0; yy < 11; ++yy)
{
    for (uint32_t xx = 0; xx < 11; ++xx)
    {
        // Rotate cube
        float mtx[16];
        bx::mtxRotateXY(mtx, time + xx*0.21f, time + yy*0.37f);
        mtx[12] = -15.0f + float(xx)*3.0f;
        mtx[13] = -15.0f + float(yy)*3.0f;
        mtx[14] = 0.0f;

        // Set model matrix for rendering.
        bgfx::setTransform(mtx);

        // Set vertex and index buffer.
        bgfx::setVertexBuffer(0, m_vbh);
        bgfx::setIndexBuffer(ibh);

        // Set render states.
        bgfx::setState(state);

        // Submit primitive for rendering to view 0.
        bgfx::submit(0, m_program);
    }
}
*/