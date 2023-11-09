#define TINYGLTF_NO_EXTERNAL_IMAGE
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"
#include <iostream>
#include <vector>

void loadgltf(const std::string& name)
{
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool ret = false;

    if (name.find(".gltf") != std::string::npos)
        ret = loader.LoadASCIIFromFile(&model, &err, &warn, name.c_str());
    else
        ret = loader.LoadBinaryFromFile(&model, &err, &warn, name.c_str());

    if (ret)
    {
        printf("File loaded succesfully\n");
        printf("Anim count: %d %s", model.animations.size(), "\n");
        printf("Mesh count: %d %s", model.meshes.size(), "\n");
        printf("Accessor count: %d %s", model.accessors.size(), "\n");
        printf("Skin count: %d %s", model.skins.size(), "\n");
        printf("Nodes count: %d %s", model.nodes.size(), "\n");
        
        printf("Animations: \n");
        for (auto anim : model.animations)
            printf("\t %s %s", anim.name.c_str(), "\n");

        printf("Models: \n");
        for (auto mesh : model.meshes)
            printf("\t %s %s", mesh.name.c_str(), "\n");

        printf("Nodes: \n");
        for (auto node : model.nodes)
            printf("\t %s %s", node.name.c_str(), "\n");
    }

    if (!warn.empty())
        printf("Warn: %s\n", warn.c_str());

    if (!err.empty())
        printf("Err: %s\n", err.c_str());

    if (!ret)
        printf("Failed to parse glTF\n");
}

int main(int argc, char** argv)
{
    //loadgltf("dagger.gltf");
    loadgltf("Knight.glb");
 
    std::cin.get();
}