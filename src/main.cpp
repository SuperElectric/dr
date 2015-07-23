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

std::string dr_directory()
{
    char* label ("DR_DIRECTORY");
    std::string directory (getenv(label));
    return directory;
}
std::string DR_DIRECTORY = dr_directory();



void rotatingMonkeyLoop()
{
    int width = 1024;
    int height = 1024;
    Display display(width,height, "Hello World!", 1, 4);
    Mesh mesh ((DR_DIRECTORY + "/assets/monkey.obj").c_str());
    ParameterVector parameters(ParameterVector::DEFAULT);
    Shader shader((DR_DIRECTORY + "/assets/basicShader").c_str());
    shader.Bind();

    float t=0, x=0, c=0, s=0;
    while(!display.IsClosed())
    {
        c = glm::cos(x);
        s = glm::sin(x);
        parameters.cameraTransformMatrix = glm::mat4( c ,0.0,-s ,0.0,
                                           0.0,1.0,0.0,0.0,
                                           s ,0.0, c ,0.0,
                                           0.0,0.0,-2.0,1.0);
        shader.Update(parameters);
        display.SetFrameBuffer(0);
        display.Clear(0.0f,0.0f,0.0f,1.0f);
        mesh.Draw();

        if ((int)t % 180  < 60)
        {
            display.SetRenderBuffer(0,0);
        }
        else if ((int)t % 180  < 120)
        {
            display.SetRenderBuffer(0,1);
        }
        else
        {
            display.SetRenderBuffer(0,3);
        }
        display.CopyFrameBuffer();
        display.Update();
        //std::cin.ignore();
        t += 1.0;
        x = 0.01*t;
    }
}

int main()
{
    // loads monkey model and basic shaders, and renders it twice, with
    // different colours, into separate offscreen framebuffers, and copies
    // one of these to the screen framebuffer. To demonstrate, the monkey
    // is shown rotating, while the framebuffer to be copied is swapped
    // every 60 frames
    rotatingMonkeyLoop();
}
