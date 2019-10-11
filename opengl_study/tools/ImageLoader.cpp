#include "ImageLoader.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb_image.h"

ImageLoader::ImageLoader(){
	// gen
	glGenTextures(1, &m_imgId);
	// bind id
	glBindTexture(GL_TEXTURE_2D, m_imgId);
}

void ImageLoader::setParameter(GLenum pname, GLint param){
	glTexParameteri(GL_TEXTURE_2D, pname, param);
}

void ImageLoader::setFlipOnLoad(bool flipOnload){
	stbi_set_flip_vertically_on_load(flipOnload);
}

bool ImageLoader::loadImg(const char* path, GLint mode){
	bool bRes = false;
	int width, heigth, nrChannels;
	unsigned char* data = stbi_load(path, &width, &heigth, &nrChannels, 0);
	if (data){
		bRes = true;
		glTexImage2D(GL_TEXTURE_2D, 0, mode, width, heigth, 0, mode, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else{
		std::cout << "Faild to load Texture" << std::endl;
	}
	stbi_image_free(data);
	return bRes;
}

bool ImageLoader::defaultLoadJPG(const char* path, bool flipOnload /*= true*/){
	setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	setParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	setFlipOnLoad(flipOnload);
	return loadImg(path, GL_RGB);
}

bool ImageLoader::defaultLoadPNG(const char* path, bool flipOnload /*= true*/) {
	setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	setParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	setFlipOnLoad(flipOnload);
	return loadImg(path, GL_RGBA);
}