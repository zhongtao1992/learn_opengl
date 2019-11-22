#include "SphereMesh.h"
#include <vector>

SphereMesh::SphereMesh(const unsigned int& xSegment, const unsigned int& ySegment){
	m_uXSegment = xSegment;
	m_uYSegment = ySegment;
	const float PI = 3.14159265359;

	glGenVertexArrays(1, &m_uMeshVAO);
	glGenBuffers(1, &m_uMeshVBO);
	glGenBuffers(1, &m_uMeshEBO);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uv;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

	for (unsigned int y = 0; y <= ySegment; ++y){
		for (unsigned int x = 0; x <= xSegment; ++x){
			float xseg = (float)x / (float)xSegment;
			float yseg = (float)y / (float)ySegment;
			float xPos = std::cos(xseg * 2.0f * PI) * std::sin(yseg * PI);
			float yPos = std::cos(yseg * PI);
			float zPos = std::sin(xseg * 2.0f * PI) * std::sin(yseg * PI);

			positions.push_back(glm::vec3(xPos, yPos, zPos));
			uv.push_back(glm::vec2(xseg, yseg));
			normals.push_back(glm::vec3(xPos, yPos, zPos));
		}
	}

	bool oddRow = false;
	for (int y = 0; y < ySegment; ++y){
		if (!oddRow) {
			for (int x = 0; x <= xSegment; ++x) {
				indices.push_back(y * (xSegment + 1) + x);
				indices.push_back((y + 1) * (xSegment + 1) + x);
			}
		}
		else {
			for (int x = xSegment; x >= 0; --x){
				indices.push_back((y + 1) * (xSegment + 1) + x);
				indices.push_back(y * (xSegment + 1) + x);
			}
		}
		oddRow = !oddRow;
	}
	m_uIndexCount = indices.size();

	std::vector<float> data;
	for (int i = 0; i < positions.size(); ++i){
		data.push_back(positions[i].x);
		data.push_back(positions[i].y);
		data.push_back(positions[i].z);
		if (uv.size() > 0){
			data.push_back(uv[i].x);
			data.push_back(uv[i].y);
		}
		if (normals.size() > 0){
			data.push_back(normals[i].x);
			data.push_back(normals[i].y);
			data.push_back(normals[i].z);
		}
	}

	glBindVertexArray(m_uMeshVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_uMeshVBO);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uMeshEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	float stride = (3 + 2 + 3) * sizeof(float);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
}

SphereMesh::~SphereMesh(){
	glDeleteVertexArrays(1, &m_uMeshVAO);
	glDeleteBuffers(1, &m_uMeshVBO);
	glDeleteBuffers(1, &m_uMeshEBO);
}

void SphereMesh::draw(GLShaderProgram shader){
	glBindVertexArray(m_uMeshVAO);
	glDrawElements(GL_TRIANGLE_STRIP, m_uIndexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void SphereMesh::setAttribute(const unsigned int& uIdx, const unsigned int& uSize, const unsigned int& uStride, const void *pointer){

}