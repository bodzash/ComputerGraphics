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
#include "ContentManager/ContentManager.h"
#include "Utility.h"

#define clog(x) std::cout << x << std::endl


static void OnGLFWError(int error, const char *description)
{
	fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    // Tangent
    // Bitangent
    // Skeletal bullshit here

    // static layout maybe here dawg
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
        Handle = bgfx::createTexture(mem, BGFX_TEXTURE_NONE | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP, 0, &textureInfo);
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

        //SetupMesh();
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
                bgfx::setTexture(1, specular, Textures[i].Handle);
            break;

            /*
            case aiTextureType_NORMALS:
                bgfx::setTexture(2, specular, Textures[i].Handle);
            */

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

    Model(const std::string& path, bgfx::VertexLayout staticVertexLayout)
    {
        LoadModel(path);


        // Setup meshes
        // passing in shit is temporary
        for (auto& mesh : Meshes)
        {
            mesh.SetupMesh(staticVertexLayout);
        }
    }

    void LoadModel(const std::string& path)
    {
        Assimp::Importer import;
        // aiProcess_FlipUVs <- if directx use this lol
        const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);
        
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
        {
            clog("ERROR::ASSIMP::" << import.GetErrorString());
            return;
        }

        Directory = path.substr(0, path.find_last_of("/"));

        ProcessNode(scene->mRootNode, scene); 
    }

    void ProcessNode(aiNode* node, const aiScene* scene)
    {
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            Meshes.push_back(ProcessMesh(mesh, scene));
        }
        
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(node->mChildren[i], scene);
        }
    }

    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene)
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

            /*
            DO A CHECK FOR TANG AND BITANG LIKE ABOVE :D
            // Tangent
            vertex.Tangent = vector;
            glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);

            // Bitangent
            vertex.Bitangent = vector;
            glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
            */

            // Texture coordinates
            if(mesh->mTextureCoords[0])
            {
                // Texcoords
                vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
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
        // if statement is fucky TODO:
        if(mesh->mMaterialIndex >= 0)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            // Diffuse maps
            std::vector<Texture> diffuseMaps = LoadTextures(material, aiTextureType_DIFFUSE);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            // Specular maps
            std::vector<Texture> specularMaps = LoadTextures(material, aiTextureType_SPECULAR);
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

            // Normal maps

            // Emission maps

            // Height maps
        }

        return Mesh(vertices, indices, textures);
    }

    std::vector<Texture> LoadTextures(aiMaterial* mat, aiTextureType type)
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

    void Render(bgfx::ViewId view, bgfx::UniformHandle diffuse, bgfx::UniformHandle specular, bgfx::ProgramHandle program)
    {
        for (auto& mesh : Meshes)
        {
            mesh.Render(diffuse, specular);
            bgfx::submit(view, program);
        }
    }

};

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

int main(int argc, char** argv)
{
    glfwSetErrorCallback(OnGLFWError);
    glfwInit();

    int WINDOW_WIDTH = 800;
    int WINDOW_HEIGHT = 600;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Renderer", nullptr, nullptr);

    bgfx::Init bgfxInit;
    bgfxInit.platformData.nwh = glfwGetWin32Window(window);
    bgfxInit.type = bgfx::RendererType::Direct3D9;
    bgfxInit.resolution.width = WINDOW_WIDTH;
    bgfxInit.resolution.height = WINDOW_HEIGHT;
    bgfxInit.resolution.reset = BGFX_RESET_VSYNC | BGFX_RESET_FLUSH_AFTER_RENDER;
    // BGFX_RESET_SRGB_BACKBUFFER
    bgfx::init(bgfxInit);

    // DEBUG
    bgfx::setDebug(BGFX_DEBUG_STATS);
    //bgfx::setDebug(BGFX_DEBUG_WIREFRAME | BGFX_DEBUG_STATS | BGFX_DEBUG_PROFILER);

    //clog(bgfx::getCaps()->limits.maxDrawCalls);
    /*
    clog(bgfx::getCaps()->homogeneousDepth);
    clog(bgfx::getCaps()->limits.maxFBAttachments);
    clog(bgfx::getCaps()->limits.maxOcclusionQueries);
    std::cout << (bool)(bgfx::getCaps()->supported & BGFX_CAPS_RENDERER_MULTITHREADED) << '\n';
    std::cout << (bool)(bgfx::getCaps()->supported & BGFX_CAPS_OCCLUSION_QUERY) << '\n';
    std::cout << (bool)(bgfx::getCaps()->supported & BGFX_CAPS_TEXTURE_BLIT) << '\n';
    std::cout << (bool)(bgfx::getCaps()->supported & BGFX_CAPS_SWAP_CHAIN) << '\n';
    std::cout << (bool)(bgfx::getCaps()->supported & BGFX_CAPS_COMPUTE) << '\n';
    */

    ContentManager::Get().Init();

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
    Model mdl("Content/Models/Angel/Skel_VoG.dae", staticVertexLayout);
    
    auto* mem = Utility::LoadBinaryData("Content/Textures/Skyboxes/SkyboxDay.dds");

    bgfx::TextureHandle skyboxTexture = bgfx::createTexture(mem, BGFX_TEXTURE_NONE | BGFX_SAMPLER_UVW_CLAMP, 0);

    /*
    bgfx::UniformHandle u_texNormal = bgfx::createUniform("s_Diffuse", bgfx::UniformType::Sampler);
    bgfx::UniformHandle u_texSpecular = bgfx::createUniform("s_Specular", bgfx::UniformType::Sampler);

    bgfx::UniformHandle u_model = bgfx::createUniform("u_Model", bgfx::UniformType::Mat4);
    bgfx::UniformHandle u_invmodel = bgfx::createUniform("u_InverseModel", bgfx::UniformType::Mat4);
    bgfx::UniformHandle u_camMatrix = bgfx::createUniform("u_ProjView", bgfx::UniformType::Mat4);
    bgfx::UniformHandle u_viewposition = bgfx::createUniform("u_ViewPosition", bgfx::UniformType::Vec4);
    bgfx::UniformHandle u_material = bgfx::createUniform("u_Material", bgfx::UniformType::Vec4, 1);
    bgfx::UniformHandle u_dlight = bgfx::createUniform("u_DirLight", bgfx::UniformType::Vec4, 4);
    */

    glm::vec3 pos = {0.0f, 0.0f, 0.0f};
    glm::vec3 orient = {1.0f, 0.0f, 0.0f};
    glm::vec3 up = {0.0f, 1.0f, 0.0f};

    float speed = 2.f;
    float sens = 100.0f;
    bool firstClick = true;

    const bgfx::ViewId SHADOW_PASS = 0;
    const bgfx::ViewId GEOMETRY_PASS = 1;
    const bgfx::ViewId LIGHTING_PASS = 2;
    // other stupid shit passes

	bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f); //0x443355FF //0x11212B
	bgfx::setViewRect(0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    Material materialData;
    materialData.Shininess = glm::vec4(32.0f);

    DirectionalLight dlightData;
    dlightData.Direction = glm::vec4(-0.2f, -1.0f, -0.3f, 1.0f);
    /*
    dlightData.Ambient = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
    dlightData.Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    dlightData.Specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    */
   
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
        
        auto lol = proj * view;

        // Model
        bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CW);

        glm::mat4 invmodel = glm::inverse(model);
        glm::vec4 viewPos(pos, 1.0f);
        bgfx::setUniform(ContentManager::Get().Uniforms.ProjView, &lol);
        bgfx::setUniform(ContentManager::Get().Uniforms.Model, &model);
        bgfx::setUniform(ContentManager::Get().Uniforms.InverseModel, &invmodel);
        bgfx::setUniform(ContentManager::Get().Uniforms.Material, &materialData.Shininess);
        bgfx::setUniform(ContentManager::Get().Uniforms.DirLight, &dlightData, 4);
        bgfx::setUniform(ContentManager::Get().Uniforms.ViewPosition, &viewPos);

        mdl.Render(0, ContentManager::Get().Uniforms.Diffuse, ContentManager::Get().Uniforms.Specular,
            ContentManager::Get().Shaders.StaticMesh);

        // Skybox

        bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_Z | BGFX_STATE_WRITE_A | BGFX_STATE_DEPTH_TEST_LEQUAL);

        glm::mat4 skyboxview = glm::mat4(glm::mat3(view));

        auto skyboxviewproj = proj * skyboxview;
        
        bgfx::setUniform(ContentManager::Get().Uniforms.Model, &model);
        bgfx::setUniform(ContentManager::Get().Uniforms.ProjView, &skyboxviewproj);

        bgfx::setVertexBuffer(0, svbo);
        bgfx::setIndexBuffer(sebo);
        bgfx::setTexture(0, ContentManager::Get().Uniforms.Diffuse, skyboxTexture);
        bgfx::submit(0, ContentManager::Get().Shaders.Skybox);
        
        bgfx::frame();
    }

    ContentManager::Get().Shutdown();
    bgfx::shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
}