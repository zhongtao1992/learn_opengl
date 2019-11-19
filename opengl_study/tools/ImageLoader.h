#pragma once
#include <glad\glad.h>

class ImageLoader{
public:
	ImageLoader();

	void setParameter(GLenum pname, GLint param);

	void setFlipOnLoad(bool flipOnload);

	bool loadImg(const char* path, GLint mode);

	bool loadImg(const char* path, bool bGammaCorrect = false);

	bool defaultLoadJPG(const char* path, bool flipOnload = true);

	bool defaultLoadPNG(const char* path, bool flipOnload = true);

	bool defaultLoadImg(const char* path, bool flipOnload = true);

	unsigned int getID() {
		return m_imgId;
	};

private:
	unsigned int m_imgId;
};