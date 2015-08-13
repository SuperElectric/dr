#ifndef SCENE_H
#define SCENE_H
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "tiny_obj_loader.h"

class Scene{
    private:
        // Define some enums that are used in constructing the shaders and the mesh
        enum {ATTRIBUTE_VERTEX_POSITIONS=0,
              ATTRIBUTE_VERTEX_TEX_COORDS=1,
              ATTRIBUTE_VERTEX_NORMALS=2};
        enum {XYZ_VB=0, UV_VB=1, NORMALS_VB=2, INDEX_VB=3, NUM_BUFFERS=4};

        // mesh stuff. Namely, gluints for the vertex attribute array
        // and the position, textures, normals, ints buffers.
        GLuint m_VAO;
        GLuint m_buffers[NUM_BUFFERS];
        unsigned int m_numIndices;
        void init(const tinyobj::shape_t &shape,
                  const std::vector<tinyobj::material_t> &materials);
};






#endif // SCENE_H
