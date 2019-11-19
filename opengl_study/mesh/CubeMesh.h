#pragma once
#include "MeshCustom.h"

class CubeMesh : MeshCustom {
public:
	CubeMesh(void* vertexData, const unsigned int& vertexSize);
	~CubeMesh();
public:
	void draw(GLShaderProgram shader) override;
	void setAttribute(const unsigned int& uIdx, const unsigned int& uSize, const unsigned int& uStride, const void *pointer) override;
};