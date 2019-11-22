#pragma once
#include "MeshCustom.h"

class SphereMesh: public MeshCustom{
public:
	SphereMesh(const unsigned int& xSegment, const unsigned int& ySegment);
	~SphereMesh();
public:
	void draw(GLShaderProgram shader) override;
	void setAttribute(const unsigned int& uIdx, const unsigned int& uSize, const unsigned int& uStride, const void *pointer) override;
private:
	unsigned int m_uMeshEBO;
	unsigned int m_uXSegment;
	unsigned int m_uYSegment;
	unsigned int m_uIndexCount;
};