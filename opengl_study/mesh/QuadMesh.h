#pragma once
#include "MeshCustom.h"

class QuadMesh: public MeshCustom{
public:
	QuadMesh(void* vertexData, const unsigned int& vertexSize);
	~QuadMesh();
public:
	void draw(GLShaderProgram shader) override;

	void drawStrip(GLShaderProgram shader);

	void setAttribute(const unsigned int& uIdx, const unsigned int& uSize, const unsigned int& uStride, const void *pointer) override;
};