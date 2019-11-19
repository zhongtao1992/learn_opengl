#include "QuadMesh.h"
QuadMesh::QuadMesh(void* vertexData, const unsigned int& vertexSize) {
	m_pVertexData = vertexData;
	m_uVertexSize = vertexSize;

	init();
}

QuadMesh::~QuadMesh() {
	glDeleteVertexArrays(1, &m_uMeshVAO);
	glDeleteBuffers(1, &m_uMeshVBO);
}

void QuadMesh::draw(GLShaderProgram shader) {
	glBindVertexArray(m_uMeshVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void QuadMesh::drawStrip(GLShaderProgram shader){
	glBindVertexArray(m_uMeshVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void QuadMesh::setAttribute(const unsigned int& uIdx, const unsigned int& uSize, const unsigned int& uStride, const void *pointer) {
	glBindVertexArray(m_uMeshVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_uMeshVBO);
	glEnableVertexAttribArray(uIdx);
	glVertexAttribPointer(uIdx, uSize, GL_FLOAT, GL_FALSE, uStride, pointer);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}