#include "Model.h"
#include <iostream>
#include <map>
#include "../tools/ImageLoader.h"
std::map<std::string, Texture> cacheTextures;

Model::Model(const char* path){
	loadModel(path);
}

void Model::Draw(GLShaderProgram shader){
	for(auto v : meshes){
		v.Draw(shader);
	}
}

void Model::DrawInstance(GLShaderProgram shader, const int& amount){
	for (auto v : meshes){
		v.DrawInstance(shader, amount);
	}
}

void Model::Release(){
	for (auto v : meshes){
		v.Release();
	}
}

void Model::loadModel(const std::string& path){
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene){
	for (unsigned int i = 0; i < node->mNumMeshes; ++i){
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for(unsigned int i = 0; i < node->mNumChildren; ++i){
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene){
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// vertex
	for(unsigned int i = 0; i < mesh->mNumVertices; ++i){
		Vertex vertex;

		// position
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		// normal
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;
		// texture
		if (mesh->mTextureCoords[0]){
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	// indice
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i){
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j){
			indices.push_back(face.mIndices[j]);
		}
	}

	// material
	if (mesh->mMaterialIndex >= 0){
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(mat, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTextures(mat, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		std::vector<Texture> reflectionMaps = loadMaterialTextures(mat, aiTextureType_AMBIENT, "texture_reflection");
		textures.insert(textures.end(), reflectionMaps.begin(), reflectionMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i) {
		aiString str;
		mat->GetTexture(type, i, &str);
		std::string path = str.C_Str();

		if (cacheTextures.find(path) != cacheTextures.end()) {
			textures.push_back(cacheTextures.find(str.C_Str())->second);
		}
		else {
			std::string loadPath = this->directory + "/" + path;
			ImageLoader loader;
			loader.loadImg(loadPath.c_str());
			Texture texture;
			texture.id = loader.getID();
			texture.type = typeName;
			texture.path = path;

			cacheTextures.insert(std::make_pair(path, texture));
			textures.push_back(texture);
		}		
	}
	return textures;
}