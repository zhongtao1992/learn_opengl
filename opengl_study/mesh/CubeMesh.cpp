#include "CubeMesh.h"

CubeMesh::CubeMesh(void* vertexData, const unsigned int& vertexSize){
	m_pVertexData = vertexData;
	m_uVertexSize = vertexSize;

	init();
}

CubeMesh::~CubeMesh(){
	glDeleteVertexArrays(1, &m_uMeshVAO);
	glDeleteBuffers(1, &m_uMeshVBO);	
}

void CubeMesh::draw(GLShaderProgram shader){
	glBindVertexArray(m_uMeshVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void CubeMesh::setAttribute(const unsigned int& uIdx, const unsigned int& uSize, const unsigned int& uStride, const void *pointer){
	glBindVertexArray(m_uMeshVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_uMeshVBO);
	glEnableVertexAttribArray(uIdx);
	glVertexAttribPointer(uIdx, uSize, GL_FLOAT, GL_FALSE, uStride, pointer);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}