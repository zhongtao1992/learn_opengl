#include "MeshCustom.h"

MeshCustom::MeshCustom(){
	m_uMeshVAO = 0;
	m_uMeshVBO = 0;

	m_pVertexData = nullptr;
	m_uVertexSize = 0;
}

MeshCustom::~MeshCustom(){

}

void MeshCustom::init(){
	// gen
	glGenVertexArrays(1, &m_uMeshVAO);
	glGenBuffers(1, &m_uMeshVBO);
	// bind
	glBindVertexArray(m_uMeshVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_uMeshVBO);
	glBufferData(GL_ARRAY_BUFFER, m_uVertexSize, m_pVertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}