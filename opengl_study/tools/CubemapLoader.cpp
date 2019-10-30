#include "CubemapLoader.h"

#include <iostream>
#include "../external/stb_image.h"

CubemapLoader::CubemapLoader(){
	glGenTextures(1, &m_Id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Id);
}

bool CubemapLoader::loadCubumap(std::vector<std::string> faces) {
	bool bRes = false;
	int width, height, nChannels;
	for (unsigned int i = 0; i < faces.size(); ++i){
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nChannels, 0);
		if (data){
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else{
			bRes = false;
			std::cout << "Cubme texture failed to load at path : " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	return bRes;
}