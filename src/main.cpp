#include <iostream>
#include <GL/glew.h>
#include "display.h"
#include "mesh.h"
#include "shader.h"

int main()
{
    Display display(1000,1000, "Hello World!");
    Vertex vertices[] = {Vertex(glm::vec3(-0.5,-0.5,0)),
                         Vertex(glm::vec3(0.5,0.5,0)),
                         Vertex(glm::vec3(0.5,-0.5,0)),
                         Vertex(glm::vec3(0.5,0.5,0)),
                         Vertex(glm::vec3(-0.5,-0.5,0)),
                         Vertex(glm::vec3(-0.5,0.5,0))};
    Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]));
    Shader shader("basicShader");
    shader.Bind();

    while(!display.IsClosed()){
        display.Clear(0.0f,0.0f,0.0f,1.0f);
        mesh.Draw();
        display.Update();
//        std::cin.ignore();
    }
}
