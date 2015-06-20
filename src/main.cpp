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
    Display display(1440,1440, "Hello World!");
    Mesh mesh ((DR_DIRECTORY + "/assets/monkey.obj").c_str());
    Shader shader((DR_DIRECTORY + "/assets/basicShader").c_str());
    float t = 0;
    while(!display.IsClosed()){
    display.Clear(0.0f,0.0f,0.0f,1.0f);
    shader.Bind();
    shader.Update(0.0001*t*t);
    mesh.Draw();
    display.Update();
    //std::cin.ignore();
    t += 1.0;
    }
}
