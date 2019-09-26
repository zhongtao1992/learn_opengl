#pragma once

class GLShaderProgram{
public:
	GLShaderProgram();
	// init with disk file
	bool initWithDiskFile(const char* vertexPath, const char* fragmentPath);
	// init with source
	bool initWithSource(const char* vertexSource, const char* fragmentSource);

	// use shader program
	void use();
	// get uniform location/index
	int getLocation(const char* name);
public: // utility uniform functions
	int setInt(const char* name, const int& nValue);
	int setFloat(const char* name, const float& fValue);
private:
	unsigned int m_programID;
	bool m_bValid;
};