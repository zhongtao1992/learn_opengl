#pragma once
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "../tools/GLShaderProgram.h"
#include "Mesh.h"

#include <vector>
#include <string>

class Model{
public:
	Model(const char* path);
	void Draw(GLShaderProgram shader);
	void DrawInstance(GLShaderProgram shader, const int& amount);
	void Release();

	std::vector<Mesh> meshes;
	std::string directory;

private:
	void loadModel(const std::string& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};