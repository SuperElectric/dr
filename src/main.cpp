#include <iostream>
#include <string>
#include <cstring>
#include <GL/glew.h>
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "tiny_obj_loader.h"


std::string DR_DIRECTORY ("/home/daniel/dr");

int main()
{
    Display display(1700,1700, "Hello World!");
    Mesh mesh ((DR_DIRECTORY + "/assets/monkey.obj").c_str());
    Shader shader((DR_DIRECTORY + "/assets/basicShader").c_str());
    //while(!display.IsClosed())
    display.Clear(0.0f,0.0f,0.0f,1.0f);
    shader.Bind();
    mesh.Draw();
    display.Update();
    std::cin.ignore();
}
