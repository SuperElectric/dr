#ifndef MESH_H
#define MESH_H
#include <glm/glm.hpp>
#include <GL/glew.h>


class Vertex{
    public:
//    Vertex(const glm::vec3 position){
//        this->position = position;
//    }
    Vertex(const glm::vec3 pos){
        this->x = pos.x;
        this->y = pos.y;
        this->z = pos.z;
    }
    private:
    //glm::vec3 position;
    GLfloat x;
    GLfloat y;
    GLfloat z;
};

class Mesh{
    public:
        Mesh(Vertex* vertices, unsigned int numVertices);
        void Draw();
        virtual ~Mesh();
    private:
        enum {POSITION_VB=0, NUM_BUFFERS=1};
        GLuint m_VAO;
        GLuint m_VAB[NUM_BUFFERS];
        unsigned int m_drawcount;


};

#endif // MESH_H
