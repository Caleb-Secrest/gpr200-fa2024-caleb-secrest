#ifndef MODEL_H
#define MODEL_H

#include "../ew/external/glad.h"
#include "../ew/external/stb_image.h"
#include "mesh.hpp"
#include "shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

unsigned int TextureFromFile(const char* path, const std::string& direct, bool gamma = false);

class Model
{
private:
    void LoadModel(std::string const& path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

public:
    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string direct;
    bool gammaCorrection;

    Model(std::string const& path, bool gamma = false) : gammaCorrection(gamma) { LoadModel(path); };
    void Draw(Shader& shdaer);

};

unsigned int TextureFromFile(const char* path, const std::string& direct, bool gamma);

#endif
