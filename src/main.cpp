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
#include "texture.h"

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
    Mesh mesh ((DR_DIRECTORY + "/assets/monkey2.obj").c_str());
    ParameterVector parameters(ParameterVector::DEFAULT);
    Shader shader((DR_DIRECTORY + "/assets/basicShader").c_str());
    shader.Bind();
    Texture texture0 ((DR_DIRECTORY + "/assets/monkey_light_0.png").c_str());
    texture0.Bind(0);
    Texture texture1 ((DR_DIRECTORY + "/assets/monkey1.png").c_str());
    texture0.Bind(1);

    float t=0, x=0, c=0, s=0;
    while(!display.IsClosed())
    {
        c = glm::cos(x);
        s = glm::sin(x);
        float elev = 0.9;
        parameters.cameraTransformMatrix = glm::mat4(1.0,0.0,0.0 ,0.0,
                                                     0.0,glm::cos(elev), glm::sin(elev),0.0,
                                                     0.0,-glm::sin(elev), glm::cos(elev) ,0.0,
                                                     0.0,0.0,-6.0,1.0)
                                         * glm::mat4( c ,0.0,-s ,0.0,
                                                     0.0,1.0,0.0,0.0,
                                                      s ,0.0, c ,0.0,
                                                     0.0,1.0,0.0,1.0);
        shader.Update(parameters);
        display.SetFrameBuffer(0);
        display.Clear(0.0f,0.0f,0.0f,1.0f);
        mesh.Draw();

        if ((int)t % 180  < 200)
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
