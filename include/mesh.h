#ifndef MESH_H
#define MESH_H
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "tiny_obj_loader.h"

class Vertex{
    public:
    Vertex(const glm::vec3 position){
        this->position = position;
    }
    private:
    glm::vec3 position;
};

class Mesh{
    public:
        //Mesh(Vertex* vertices, unsigned int numVertices);
        Mesh(char* fileName, char* mtlBasePath=NULL, int object=0);
        Mesh(const tinyobj::shape_t &shape,
             const std::vector<tinyobj::material_t> &materials);
        void Draw();
        virtual ~Mesh();
    private:
        enum {XYZ_VB=0, UV_VB=1, NORMALS_VB=2, INDEX_VB=3, NUM_BUFFERS=4};
        GLuint m_VAO;
        GLuint m_buffers[NUM_BUFFERS];
        unsigned int m_numIndices;
        void init(const tinyobj::shape_t &shape,
                  const std::vector<tinyobj::material_t> &materials);
};

#endif // MESH_H
