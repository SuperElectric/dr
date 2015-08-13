#ifndef MESH_H
#define MESH_H
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "tiny_obj_loader.h"

class Mesh{
    public:
        //Mesh(Vertex* vertices, unsigned int numVertices);
        Mesh(const char* fileName, char* mtlBasePath=NULL, int object=0);
        Mesh(const tinyobj::shape_t &shape,
             const std::vector<tinyobj::material_t> &materials);
        // Get rid of this method.
        void Draw();
        virtual ~Mesh();
        enum {ATTRIBUTE_VERTEX_POSITIONS,
              ATTRIBUTE_VERTEX_TEX_COORDS,
              ATTRIBUTE_VERTEX_NORMALS};
    private:
        enum {XYZ_VB=0, UV_VB=1, NORMALS_VB=2, INDEX_VB=3, NUM_BUFFERS=4};
        GLuint m_VAO;
        GLuint m_buffers[NUM_BUFFERS];
        unsigned int m_numIndices;
        void init(const tinyobj::shape_t &shape,
                  const std::vector<tinyobj::material_t> &materials);
};



#endif // MESH_H
