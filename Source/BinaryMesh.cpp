#include <iostream>
#include <fstream>
#include <vector>

struct Vertex
{
    float px = 0.0f;
    float py = 0.0f;
    float pz = 0.0f;
    float tu = 0.0f;
    float tv = 0.0f;
    //glm::vec3 Position;
    //glm::vec2 TexCoord;

    friend std::ostream& operator<<(std::ostream& os, const Vertex& vert);
};

std::ostream& operator<<(std::ostream& os, const Vertex& vert)
{
    os << vert.px << " " << vert.py << " " << vert.pz << " " << vert.tu << " " << vert.tv;
    return os;
}

struct Mesh
{
    std::vector<Vertex> Verticies;
    std::vector<uint16_t> Indicies;

    friend std::ostream& operator<<(std::ostream& os, const Mesh& mesh);
};

std::ostream& operator<<(std::ostream& os, const Mesh& mesh)
{
    os << "Verticies:" << std::endl;
    for (size_t i = 0; i < mesh.Verticies.size(); i++)
    {
        os << " " << mesh.Verticies[i] << std::endl;
    }

    os << "Indicies:" << std::endl;
    for (size_t i = 0; i < mesh.Indicies.size(); i++)
    {
        os << " " << mesh.Indicies[i] << std::endl;
    }

    return os;
}

bool SerializeMesh(const std::string& path, Mesh& mesh)
{
    std::fstream fout;
    fout.open(path, std::ios::out | std::ios::binary);

    if (fout)
    {
        // Version
        uint32_t exporterVersion = 1;
        fout.write(reinterpret_cast<char*>(&exporterVersion), sizeof(uint32_t));

        // Number of Verticies
        uint32_t numVerticies = mesh.Verticies.size();
        fout.write(reinterpret_cast<char*>(&numVerticies), sizeof(uint32_t));

        // Verticies Data
        fout.write(reinterpret_cast<char*>(mesh.Verticies.data()), numVerticies * sizeof(Vertex));

        // Number of Indicies
        uint32_t numIndicies = mesh.Indicies.size();
        fout.write(reinterpret_cast<char*>(&numIndicies), sizeof(uint32_t));

        // Indicies Data
        fout.write(reinterpret_cast<char*>(mesh.Indicies.data()), numIndicies * sizeof(uint16_t));

        // Close file
        fout.close();

        return true;
    }
    else
    {
        return false;
    }
}

bool DeserializeMesh(const std::string& path, Mesh& mesh)
{
    std::fstream fin;
    fin.open(path, std::ios::in | std::ios::binary);

    if (fin)
    {
        // Version
        uint32_t exporterVersion = 0;
        fin.read(reinterpret_cast<char*>(&exporterVersion), sizeof(uint32_t));
        std::cout << "exporterVersion: " << exporterVersion << std::endl;
        
        // Number of Verticies
        uint32_t numVerticies = 0;
        fin.read(reinterpret_cast<char*>(&numVerticies), sizeof(uint32_t));
        mesh.Verticies.resize(numVerticies);
        std::cout << "numVerticies: " << numVerticies << std::endl;

        // Verticies Data
        fin.read(reinterpret_cast<char*>(&mesh.Verticies[0]), numVerticies * sizeof(Vertex));
        
        // Number of Indicies
        uint32_t numIndicies = 0;
        fin.read(reinterpret_cast<char*>(&numIndicies), sizeof(uint32_t));
        mesh.Indicies.resize(numIndicies);
        std::cout << "numIndicies: " << numIndicies << std::endl;

        // Indicies Data
        fin.read(reinterpret_cast<char*>(&mesh.Indicies[0]), numIndicies * sizeof(uint16_t));

        // Close file
        fin.close();

        return true;
    }
    else
    {
        return false;
    }
}

int main()
{
    Mesh mesh;
    Vertex vert;
    vert.px = 1.0f;
    vert.py = 2.0f;
    vert.pz = 3.0f;
    vert.tu = 4.0f;
    vert.tv = 5.0f;
    mesh.Verticies.push_back(vert);
    mesh.Verticies.emplace_back();
    Vertex vert2;
    vert2.px = 6.0f;
    vert2.py = 7.0f;
    vert2.pz = 8.0f;
    vert2.tu = 9.0f;
    vert2.tv = 10.0f;
    mesh.Verticies.push_back(vert2);

    mesh.Indicies.push_back(2035);
    mesh.Indicies.push_back(3);
    mesh.Indicies.push_back(1456);
    mesh.Indicies.push_back(1);
    mesh.Indicies.push_back(0);
    mesh.Indicies.push_back(4);

    bool successWrite = SerializeMesh("./Test.msh", mesh);
    if (successWrite)
    {
        std::cout << "Write Success!" << std::endl;
    }

    Mesh readMesh;

    bool successRead = DeserializeMesh("./Test.msh", readMesh);
    if (successRead)
    {
        std::cout << "Read Success!" << std::endl;
        std::cout << readMesh;
    }

    std::cin.get();
}