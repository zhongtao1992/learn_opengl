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

bool ImageLoader::loadImg(const char* path, bool bGammaCorrect /*= false*/){
	bool bRes = false;
	int width, heigth, nrChannels;
	unsigned char* data = stbi_load(path, &width, &heigth, &nrChannels, 0);
	if (data) {
		bRes = true;
		GLenum internalFormat;
		GLenum dataFormat;
		if (nrChannels == 1) {
			internalFormat = dataFormat = GL_RED;
		}
		else if (nrChannels == 3) {
			internalFormat = bGammaCorrect ? GL_SRGB : GL_RGB;
			dataFormat = GL_RGB;
		}
		else if (nrChannels == 4) {
			internalFormat = bGammaCorrect ? GL_SRGB_ALPHA : GL_RGBA;
			dataFormat = GL_RGBA;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, heigth, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
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

bool ImageLoader::defaultLoadImg(const char* path, bool flipOnload /*= true*/){
	setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	setParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	setFlipOnLoad(flipOnload);
	return loadImg(path);
}

bool ImageLoader::loadHDR(const char* path){
	bool bRes = false;
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrComponents;
	float* data = stbi_loadf(path, &width, &height, &nrComponents, 0);
	if(data){
		bRes = true;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

		setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
		setParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
		setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else{
		std::cout << "Failed to load HDR image." << std::endl;
	}
	stbi_image_free(data);
	return bRes;
}