#pragma once
#include<iostream>
#include"Mesh.h"
#include<assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;

	Model(std::string const& path);
    void draw(Shader* shader);

private:
    
    void loadModel(std::string const &path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene);
    unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
    unsigned int GenerateTex(unsigned char* data, int width, int height, int nrComponents, bool gama=false);
    unsigned int TextureFrom_FBX_EmbeddedTexture(const aiTexture* aiTex, bool gama = false);
};

