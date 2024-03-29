#include "mesh.h"
#include <iostream>


Mesh::Mesh(const char* fileName, char* mtlBasePath, int object)
{
    std::vector <tinyobj::shape_t> shapes;
    std::vector <tinyobj::material_t> materials;
    tinyobj::LoadObj(shapes,materials,fileName);
    if (shapes.empty()){
        std::cerr << "Invalid .obj file" << std::endl;
        return;
    }
    init(shapes[object], materials);
}


Mesh::Mesh(const tinyobj::shape_t &shape,
           const std::vector<tinyobj::material_t> &materials)
{
    init(shape, materials);
}

Mesh::Mesh()
{
    float positions[12] = {1.0,1.0,0.0,
                           -1.0,1.0,0.0,
                           -1.0,-1.0,0.0,
                           1.0,-1.0,0.0};
    float texcoords[8] = {1.0,1.0,
                          0.0,1.0,
                          0.0,0.0,
                          1.0,0.0};
    float normals[12] = {0.0,0.0,0.0,
                         0.0,0.0,0.0,
                         0.0,0.0,0.0,
                         0.0,0.0,0.0};
    int indices[6] = {0,2,1,0,3,2};
    m_numIndices = 6;
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    glGenBuffers(NUM_BUFFERS, m_buffers);

    GLint currentBufferBinding;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currentBufferBinding);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[XYZ_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0])*12,
                 positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(ATTRIBUTE_VERTEX_POSITIONS);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[UV_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords[0])*8,
                 texcoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(ATTRIBUTE_VERTEX_TEX_COORDS);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[NORMALS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0])*12,
                 normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(ATTRIBUTE_VERTEX_NORMALS);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[INDEX_VB]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0])*6,
                 indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, currentBufferBinding);
    glBindVertexArray(0);

    m_isSquare = true;
}

Mesh::~Mesh()
{
    glDeleteBuffers(NUM_BUFFERS, m_buffers);
	glDeleteVertexArrays(1, &m_VAO);
}


void Mesh::init(const tinyobj::shape_t &shape,
                  const std::vector<tinyobj::material_t> &materials)
{
    const std::vector<float>& positions = shape.mesh.positions;
    const std::vector<float>& texcoords = shape.mesh.texcoords;
    const std::vector<float>& normals = shape.mesh.normals;
    const std::vector<unsigned int>& indices = shape.mesh.indices;
    m_numIndices = indices.size();
    // create buffers and copy positions, uvs, normals, indices, and
    // (not yet) material parameters
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    glGenBuffers(NUM_BUFFERS, m_buffers);

    GLint currentBufferBinding;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currentBufferBinding);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[XYZ_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0])*positions.size(),
                 &positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(ATTRIBUTE_VERTEX_POSITIONS);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[UV_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords[0])*texcoords.size(),
                 &texcoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(ATTRIBUTE_VERTEX_TEX_COORDS);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[NORMALS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0])*normals.size(),
                 &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(ATTRIBUTE_VERTEX_NORMALS);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[INDEX_VB]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0])*indices.size(),
                 &indices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, currentBufferBinding);
    glBindVertexArray(0);

    m_isSquare = false;
}


void Mesh::Draw()
{
	glBindVertexArray(m_VAO);
//	glEnable(GL_POLYGON_SMOOTH);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glHint(GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[INDEX_VB]);
	glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
