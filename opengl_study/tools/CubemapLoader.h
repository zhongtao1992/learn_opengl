#pragma once
#include <glad\glad.h>
#include <vector>
#include <string>

//#include "stbi_header.h"

class CubemapLoader{
public:
	CubemapLoader();

	unsigned int getID() { return m_Id; }

	bool loadCubumap(std::vector<std::string> faces);
private:
	unsigned int m_Id;
};