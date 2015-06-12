#include "mesh.h"

Mesh::Mesh(Vertex* vertices, unsigned int numVertices)
{
    m_drawcount = numVertices;
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    glGenBuffers(NUM_BUFFERS, m_VAB);
    glBindBuffer(GL_ARRAY_BUFFER, m_VAB[POSITION_VB]);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    glDeleteBuffers(NUM_BUFFERS, m_VAB);
	glDeleteVertexArrays(1, &m_VAO);
}

void Mesh::Draw()
{
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, m_drawcount);
	glBindVertexArray(0);
}
