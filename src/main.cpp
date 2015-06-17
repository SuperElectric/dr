#include <iostream>
#include <GL/glew.h>
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "tiny_obj_loader.h"


int main()
{
    Display display(502,502, "Hello World!");
    Mesh mesh ("../assets/monkey.obj");
    Shader shader("../assets/basicShader");
    //while(!display.IsClosed())
    display.Clear(0.0f,0.0f,0.0f,1.0f);
    shader.Bind();
    mesh.Draw();
    display.Update();
    std::cin.ignore();
}
