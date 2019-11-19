#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "../tools/GLShaderProgram.h"

class MeshCustom {
public:
	MeshCustom();
	virtual ~MeshCustom();

	void init();

	virtual void draw(GLShaderProgram shader) = 0;
	virtual void setAttribute(const unsigned int& uIdx, const unsigned int& uSize, const unsigned int& uStride, const void *pointer) = 0;
public:
	unsigned int m_uMeshVAO;
	unsigned int m_uMeshVBO;
	void* m_pVertexData;
	unsigned int m_uVertexSize;
};