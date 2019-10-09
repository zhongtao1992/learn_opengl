#include "GLShaderProgram.h"

#include <iostream>

std::string getStringFromFile(const char* filePath){
	FILE* fp = fopen(filePath, "rb");
	if (nullptr == fp){
		return "";
	}
	auto descriptor = _fileno(fp);
	struct stat statBuf;
	if (fstat(descriptor, &statBuf) == -1){
		fclose(fp);
		return "";
	}
	unsigned char* buffer = (unsigned char*)malloc(statBuf.st_size * sizeof(unsigned char));
	auto readSize = fread(buffer, sizeof(unsigned char), statBuf.st_size, fp);
	fclose(fp);
	if (readSize < statBuf.st_size){
		buffer[readSize] = '\0';
	}
	std::string ret((const char*)buffer);
	free(buffer);

	return ret;
}

GLShaderProgram::GLShaderProgram(){

}

bool GLShaderProgram::initWithDiskFile(const char* vertexPath, const char* fragmentPath){
	std::string vertexS = getStringFromFile(vertexPath);
	std::string fragmentS = getStringFromFile(fragmentPath);
	if (vertexS == "" || fragmentS == ""){
		return false;
	}

	return initWithSource(vertexS.c_str(), fragmentS.c_str());
}

bool GLShaderProgram::initWithSource(const char* vertexSource, const char* fragmentSource){
	// build and compile shader
	// --------------------------------
	// create vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// attach shader source
	glShaderSource(vertexShader, 1, &vertexSource, nullptr);
	// compile shader
	glCompileShader(vertexShader);
	// check error
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// create fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// build and link shaderprogram
	// --------------------------------
	// create shader program
	m_programID = glCreateProgram();
	// attach shader and link program
	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);
	glLinkProgram(m_programID);
	// check error
	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_programID, 512, nullptr, infoLog);
		std::cout << "ERROR::PROGRAM::ATTACH_LINK_FAILED\n" << infoLog << std::endl;
	}
	// delete shader 
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return true;
}

int GLShaderProgram::getLocation(const char* name){
	return glGetUniformLocation(m_programID, name);
}

void GLShaderProgram::use(){
	glUseProgram(m_programID);
}

int GLShaderProgram::setInt(const char* name, const int& nValue){
	int location = getLocation(name);
	if (location != -1){
		glUniform1i(location, nValue);
	}
	return location;
}

int GLShaderProgram::setFloat(const char* name, const float& fValue){
	int location = getLocation(name);
	if (location != -1){
		glUniform1f(location, fValue);
	}
	return location;
}

int GLShaderProgram::setMatrix4fv(const char* name, const GLfloat* pValue){
	int location = getLocation(name);
	if (location != -1){
		glUniformMatrix4fv(location, 1, GL_FALSE, pValue);
	}
	return location;
}

int GLShaderProgram::setVec3(const char* name, const float& x, const float& y, const float& z){
	int location = getLocation(name);
	if (location != -1) {
		glUniform3f(location, x, y, z);
	}
	return location;
}
