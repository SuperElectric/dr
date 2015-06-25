#include <iostream>
#include <string>
#include <cstring>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "tiny_obj_loader.h"
#include "parameterVector.h"


std::string DR_DIRECTORY ("/home/daniel/dr");

int main()
{
    GLint _maxTextureSize;
    glGetIntegerv(GL_MAX_DRAW_BUFFERS,&_maxTextureSize);
    std::cout<< "GL_MAX_DRAW_BUFFERS : %d\n" << _maxTextureSize << std::endl;
    Display display(1024,1024, "Hello World!", 2);
    Mesh mesh ((DR_DIRECTORY + "/assets/monkey.obj").c_str());
    Shader shader((DR_DIRECTORY + "/assets/basicShader").c_str());
    shader.Bind();
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
    c = glm::cos(x);
    s = glm::sin(x);
    parameters.cameraTransformMatrix = glm::mat4( c ,0.0,-s ,0.0,
                                                0.0,1.0,0.0,0.0,
                                                 s ,0.0, c ,0.0,
                                                0.0,0.0,-2.0,1.0);
    parameters.cameraRotationMatrix = glm::mat3(parameters.cameraTransformMatrix);
    parameters.materialDiffuseColour = glm::vec3(1.0,0.0,0.0);
    shader.Update(parameters);
    display.SetFrameBuffer(0);
    display.Clear(0.0f,0.0f,0.0f,1.0f);
    mesh.Draw();
    parameters.materialDiffuseColour = glm::vec3(0.0,1.0,0.0);
    shader.Update(parameters);
    display.SetFrameBuffer(1);
    display.Clear(0.0f,0.0f,0.0f,1.0f);
    mesh.Draw();

    if ((int)t % 120  < 60){
        display.SetFrameBuffer(0);
    }
    else{
        display.SetFrameBuffer(1);
    }

    display.CopyFrameBuffer();
    display.Update();
    //std::cin.ignore();
    t += 1.0;
    x = 0.01*t;
    }
}
