#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "../tools/GLShaderProgram.h"

#include <string>
#include <vector>

// ∂•µ„ Ù–‘
struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

// Œ∆¿Ì
struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh{
public:
	// function
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(GLShaderProgram shader);
	void Release();
	// mesh data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

private:
	// render data
	unsigned int VAO, VBO, EBO;
	// function
	void setupMesh();
};