#ifndef MESH_H
#define MESH_H

#include "../ew/external/glad.h"
#include "shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

#define MAX_BONE_INFLUENCE 4

struct Vertex
{
    glm::vec3 Pos;
    glm::vec3 Norm;
    glm::vec3 TexCoords;
    glm::vec3 Tang;
    glm::vec3 Bitang;
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh
{
private:
    unsigned int VBO, EBO;

    void SetupMesh();

public:
    std::vector<Vertex> verts;
    std::vector<unsigned int> indices;
    std::vector<Texture> texts;
    unsigned int VAO;

    Mesh(std::vector<Vertex> verts, std::vector<unsigned int> indices, std::vector<Texture> texts);

    void Draw(Shader& shader);

};

#endif
