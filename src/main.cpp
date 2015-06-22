#include <iostream>
#include <string>
#include <cstring>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "tiny_obj_loader.h"
#include "parameterVector.h"


std::string DR_DIRECTORY ("/home/daniel/dr");

int main()
{
    Display display(1440,1440, "Hello World!");
    Mesh mesh ((DR_DIRECTORY + "/assets/monkey.obj").c_str());
    Shader shader((DR_DIRECTORY + "/assets/basicShader").c_str());
    float zNear = 0.1;
    float zFar = 100.0;
    ParameterVector parameters;
    parameters.cameraProjectionMatrix = glm::perspective(80.0f, 1.0f, zNear, zFar);
    parameters.lightPosition = glm::vec3(0.0,0.0,2.0);
    parameters.lightColour = glm::vec3(1.0,1.0,1.0);
    parameters.materialDiffuseColour = glm::vec3(1.0,0.0,0.0);
    parameters.materialSpecularColour = glm::vec3(1.0,1.0,1.0);
    parameters.materialSpecularHardness = 10.0;

    float t=0, x=0, c=0, s=0;
    while(!display.IsClosed()){
    display.Clear(0.0f,0.0f,0.0f,1.0f);
    shader.Bind();
    c = glm::cos(x);
    s = glm::sin(x);
    parameters.cameraTransformMatrix = glm::mat4( c ,0.0,-s ,0.0,
                                                0.0,1.0,0.0,0.0,
                                                 s ,0.0, c ,0.0,
                                                0.0,0.0,-2.0,1.0);
    parameters.cameraRotationMatrix = glm::mat3(parameters.cameraTransformMatrix);
    shader.Update(parameters);
    mesh.Draw();
    display.Update();
    //std::cin.ignore();
    t += 1.0;
    x = 0.01*t;
    }
}
