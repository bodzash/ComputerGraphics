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

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

#define clog(x) std::cout << x << std::endl

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

// Shader name WITHOUT the extension (.bvshader, .bfshader, .bvs, .bfs)
bgfx::ProgramHandle LoadShaderProgram(const std::string& filePath)
{
    bgfx::ShaderHandle vsh = LoadShader("Basic.bvs");
    bgfx::ShaderHandle fsh = LoadShader("Basic.bfs");
    return bgfx::createProgram(vsh, fsh, true);
}

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    // Tangent
    // Bitangent
    // Skeletal bullshit here
};

struct Texture
{
    std::string Dir;
    std::string Path;
    aiTextureType Type;
    bgfx::TextureHandle Handle;

    Texture(const std::string& dir, const std::string& path, aiTextureType type)
        : Dir(dir), Path(path), Type(type)
    {
        // Load shit right here fuck it
        std::string fullPath = dir + "/" + path;

        // Nifty loading
        FILE* f = fopen(fullPath.c_str(), "rb");
        fseek(f, 0, SEEK_END);
        const bgfx::Memory* mem = bgfx::alloc(ftell(f));
        fseek(f, 0, SEEK_SET);
        fread(mem->data, mem->size, 1, f);
        fclose(f);

        bgfx::TextureInfo textureInfo;
        Handle = bgfx::createTexture(mem, BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE, 0, &textureInfo);
    }
};

struct Mesh
{
    std::vector<Vertex> Vertices; // probably not needed
    std::vector<uint16_t> Indicies; // probably not needed
    std::vector<Texture> Textures;

    bgfx::VertexBufferHandle VBO;
    bgfx::IndexBufferHandle EBO;

    Mesh(std::vector<Vertex> vertices, std::vector<uint16_t> indicies, std::vector<Texture> textures)
    {
        // Could use initializer list
        Vertices = vertices;
        Indicies = indicies;
        Textures = textures;

        // SetupMesh();
    }

    void SetupMesh(bgfx::VertexLayout& layout)
    {
        // Generate the bgfx shit here.
        VBO = bgfx::createVertexBuffer(bgfx::makeRef(Vertices.data(), sizeof(Vertex) * Vertices.size()), layout);
        EBO = bgfx::createIndexBuffer(bgfx::makeRef(Indicies.data(), sizeof(uint16_t) * Indicies.size()));
    }

    // input: Shader& shader or program or some stupid shit
    // TEMPORARY INPUT UNIFORM SHIT SHOULD BE STORED IN Shader class in Model or some stupid shit
    void Render(bgfx::UniformHandle diffuse, bgfx::UniformHandle specular)
    {
        // Bind buffers
        bgfx::setVertexBuffer(0, VBO);
        bgfx::setIndexBuffer(EBO);

        // Bind textures
        
        for (unsigned int i = 0; i < Textures.size(); i++)
        {
            switch (Textures[i].Type)
            {
            case aiTextureType_DIFFUSE:
                bgfx::setTexture(0, diffuse, Textures[i].Handle);
            break;

            case aiTextureType_SPECULAR:
                bgfx::setTexture(0, specular, Textures[i].Handle);
            break;
            
            default:
                break;
            }
        }
        
    }
};

struct Model
{
    // bgfx::Program Shader;
    std::vector<Mesh> Meshes;
    std::string Directory;

    std::vector<Texture> LoadedTextures;

    Model(const std::string& path)
    {
        loadModel(path);
    }

    void loadModel(const std::string& path)
    {
        Assimp::Importer import;
        const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
        
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
        {
            clog("ERROR::ASSIMP::" << import.GetErrorString());
            return;
        }

        Directory = path.substr(0, path.find_last_of("/"));

        processNode(scene->mRootNode, scene);
    }

    void processNode(aiNode* node, const aiScene* scene)
    {
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            Meshes.push_back(processMesh(mesh, scene));
        }
        
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh processMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<uint16_t> indices;
        std::vector<Texture> textures;

        // Process vertex data
        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;

            // Positions
            vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

            // Normals
            if (mesh->HasNormals())
                vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            else
                vertex.Normal = glm::vec3(1.0f);

            // Texture coordinates
            if(mesh->mTextureCoords[0])
            {
                // Texcoords
                vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
                /*
                // Tangent
                vertex.Tangent = vector;

                // Bitangent
                vertex.Bitangent = vector;
                */
            }
            else
                vertex.TexCoords = glm::vec2(0.0f);

            vertices.push_back(vertex);
        }

        // Process indices
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];

            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        
        // Process materials
        // if statement is fucky TODO
        if(mesh->mMaterialIndex >= 0)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            // Diffuse maps
            std::vector<Texture> diffuseMaps = loadTextures(material, aiTextureType_DIFFUSE);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            // Specular maps
            std::vector<Texture> specularMaps = loadTextures(material, aiTextureType_SPECULAR);
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

            // Normal maps

            // Emission maps

            // Height maps
        }

        return Mesh(vertices, indices, textures);
    }

    std::vector<Texture> loadTextures(aiMaterial* mat, aiTextureType type)
    {
        std::vector<Texture> textures;

        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            std::string newString(str.C_Str());
            std::string substr = ".png";
            newString.replace(newString.find(substr), substr.size(), ".dds");
            clog(newString);

            // Prevent duplicates
            bool skip = false;

            for (unsigned int j = 0; j < LoadedTextures.size(); j++)
            {
                if (std::strcmp(LoadedTextures[j].Path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(LoadedTextures[j]);
                    skip = true;
                    break;
                }
            }

            if (!skip)
            {
                // Not loaded yet
                Texture tex(Directory, newString, type);
                textures.push_back(tex);
                LoadedTextures.push_back(tex);
            }
        }

        return textures;
    }

    void Render(bgfx::UniformHandle diffuse, bgfx::UniformHandle specular, bgfx::ProgramHandle program)
    {
        for (auto& mesh : Meshes)
        {
            mesh.Render(diffuse, specular);
            bgfx::submit(0, program);
        }
    }

};

// This is some stupid shit
struct Material
{
    glm::vec4 Diffuse;
    glm::vec4 Specular;
    //glm::vec4 Normal;
    //glm::vec4 Emission;
    glm::vec4 Shininess;
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
	
    glm::vec4 Attenuation; // x=Constant, y=Linear, z=Quadratic
};

struct SpotLight
{
    glm::vec4 Position;
    glm::vec4 Direction;

    glm::vec4 CutOff; // x=cutoff, y=outercutoff

    glm::vec4 Ambient;
    glm::vec4 Diffuse;
    glm::vec4 Specular;

    glm::vec4 Attenuation; // x=Constant, y=Linear, z=Quadratic
};

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
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .end();

    Model mdl("Jack/HandsomeJack.dae");

    for (auto& mesh : mdl.Meshes)
    {
        mesh.SetupMesh(vertexLayout);
    }

    bgfx::UniformHandle u_texNormal = bgfx::createUniform("s_Albedo", bgfx::UniformType::Sampler);
    bgfx::UniformHandle u_texSpecular = bgfx::createUniform("s_Specular", bgfx::UniformType::Sampler);

    bgfx::UniformHandle u_model = bgfx::createUniform("u_Model", bgfx::UniformType::Mat4);
    bgfx::UniformHandle u_invmodel = bgfx::createUniform("u_InverseModel", bgfx::UniformType::Mat4);
    bgfx::UniformHandle u_viewposition = bgfx::createUniform("u_ViewPosition", bgfx::UniformType::Vec4);
    bgfx::UniformHandle u_material = bgfx::createUniform("u_Material", bgfx::UniformType::Vec4, 1);
    bgfx::UniformHandle u_light = bgfx::createUniform("u_Light", bgfx::UniformType::Vec4, 4);
    bgfx::UniformHandle u_dlight = bgfx::createUniform("u_DirLight", bgfx::UniformType::Vec4, 4);
    bgfx::UniformHandle u_plight = bgfx::createUniform("u_PointLight", bgfx::UniformType::Vec4, 5);
    bgfx::UniformHandle u_slight = bgfx::createUniform("u_SpotLight", bgfx::UniformType::Vec4, 7);

    bgfx::UniformHandle u_numplight = bgfx::createUniform("u_NumPointLight", bgfx::UniformType::Vec4, 1);
    glm::vec4 numpLights = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

    glm::vec3 pos = {0.0f, 0.0f, 2.0f};
    glm::vec3 orient = {0.0f, 0.0f, -1.0f};
    glm::vec3 up = {0.0f, 1.0f, 0.0f};

    float speed = 2.f;
    float sens = 100.0f;
    bool firstClick = true;

    bgfx::UniformHandle u_camMatrix = bgfx::createUniform("u_ProjView", bgfx::UniformType::Mat4);

    const bgfx::ViewId kClearView = 0;
	bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH /*| BGFX_CLEAR_STENCIL*/, 0x443355FF, 1.0f, 0); //0x443355FF //0x11212B
	bgfx::setViewRect(kClearView, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glm::vec4 lightPosition = glm::vec4(1.0f, 5.0f, 5.0f, 1.0f);

    Material materialData;
    materialData.Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    materialData.Specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    materialData.Shininess = glm::vec4(32.0f, 1.0f, 1.0f, 1.0f);

    DirectionalLight dlightData;
    dlightData.Direction = glm::vec4(-0.2f, -1.0f, -0.3f, 1.0f);
    dlightData.Ambient = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
    dlightData.Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    dlightData.Specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    PointLight pLightData;
    pLightData.Position = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    pLightData.Ambient = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
    pLightData.Diffuse = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    pLightData.Specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    pLightData.Attenuation = glm::vec4(1.0, 0.35, 0.44f, .0f);

    SpotLight sLightData;
    sLightData.Position = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    sLightData.Direction = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
    sLightData.CutOff = glm::vec4(glm::cos(glm::radians(15.5f)), glm::cos(glm::radians(25.5f)), 1.0f, 1.0f);
    sLightData.Ambient = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
    sLightData.Diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
    sLightData.Specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    sLightData.Attenuation = glm::vec4(1.0, 0.09, 0.032f, .0f);

    std::vector<PointLight> pLights;
    pLights.emplace_back(pLightData);
    //pLightData.Diffuse = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    //pLights.emplace_back(pLightData);

    
    glm::mat4 model{1.f};
    glm::mat4 view{1.f};
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
    
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

        #pragma region Controlls

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

        #pragma endregion Controlls

        glm::mat4 proj{1.f};

        view = glm::lookAt(pos, pos + orient, up);
        proj = glm::perspective(glm::radians(63.f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 50.f);

        if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            model = glm::translate(model, glm::vec3(-.1f, 0.0f, 0.0f));

        if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            model = glm::translate(model, glm::vec3(.1f, 0.0f, 0.0f));

        auto lol = proj * view;

        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            pLights[0].Position = glm::vec4(pos, 1.0f);
        
        glm::mat4 invmodel = glm::inverse(model);
        glm::vec4 viewPos(pos, 1.0f);

        sLightData.Position = glm::vec4(pos, 1.0f);
        sLightData.Direction = glm::vec4(orient, 0.0f);

        bgfx::setUniform(u_camMatrix, &lol, 1);
        bgfx::setUniform(u_model, &model, 1);
        bgfx::setUniform(u_invmodel, &invmodel, 1);
        bgfx::setUniform(u_viewposition, &viewPos, 1);
        bgfx::setUniform(u_material, &materialData.Shininess, 1);
        bgfx::setUniform(u_dlight, &dlightData, 4);
        //numpLights.x = pLights.size();
        //bgfx::setUniform(u_numplight, &numpLights);
        //bgfx::setUniform(u_plight, pLights.data(), 5 * numpLights.x);
        //bgfx::setUniform(u_slight, &sLightData, 7);

        mdl.Render(u_texNormal, u_texSpecular, program);
        
        bgfx::frame();
    }

    bgfx::shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
}