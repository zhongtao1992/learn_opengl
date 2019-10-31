#pragma once
#include <glad\glad.h>
#include <glm\glm.hpp>

class GLShaderProgram{
public:
	GLShaderProgram();
	// init with disk file
	bool initWithDiskFile(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	// init with source
	bool initWithSource(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);

	// use shader program
	void use();
	// get uniform location/index
	int getLocation(const char* name);
	// id
	unsigned int getID() { return m_programID; }
public: // utility uniform functions
	int setInt(const char* name, const int& nValue);
	int setFloat(const char* name, const float& fValue);
	int setMatrix4fv(const char* name, const GLfloat* pValue);
	int setVec3(const char* name, const float& x, const float& y, const float& z);
	int setVec3(const char* name, const glm::vec3& vec3Value);
private:
	unsigned int m_programID;
	bool m_bValid;
};