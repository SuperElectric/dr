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
//#include "scene.h"

void rotatingMonkeyLoop()
{
    char* label ("DR_DIRECTORY");
    std::string DR_DIRECTORY (getenv(label));

     //temporary white pixels
        float* data = new float[10000000];
        for (int i=0; i<10000000; i++){
            data[i] = 1.0;
        }


    int width = 1024;
    int height = 1024;
    Display display(width,height, "Hello World!", 1, 4);
    Mesh mesh ((DR_DIRECTORY + "/assets/monkey2.obj").c_str());
    ParameterVector parameters(ParameterVector::DEFAULT);
    Shader shader(DR_DIRECTORY + "/assets/basicShader");
    shader.Bind();
    Texture texture0 ((DR_DIRECTORY + "/assets/monkey_light_0.png").c_str());
    texture0.Bind(0, "sampler_0");
    Texture texture1 ((DR_DIRECTORY + "/assets/monkey_light_1.png").c_str());
    texture1.Bind(1, "sampler_1");
    Texture texture2 ((DR_DIRECTORY + "/assets/monkey_light_2.png").c_str());
    texture2.Bind(2, "sampler_2");
    Texture texture3 ((DR_DIRECTORY + "/assets/monkey_light_3.png").c_str());
    texture3.Bind(3, "sampler_3");

    float t=0, x=0, c=0, s=0;
    while(!display.IsClosed())
    {
        shader.Bind();
        c = glm::cos(x);
        s = glm::sin(x);
        float elev = 0.5;
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
        texture0.Bind(0, "sampler_0");
        texture1.Bind(1, "sampler_1");
        texture2.Bind(2, "sampler_2");
        texture3.Bind(3, "sampler_3");
        mesh.Draw();

        {
            int i=0;
            int n=0;
            if ((int)t % 180 < 200)
            {
                //display.SetRenderBuffer(0,0);
                i=0;
            }
            else if ((int)t % 180 < 120)
            {
//                display.SetRenderBuffer(0,1);
                i=1;
            }
            else
            {
//                display.SetRenderBuffer(0,2);
                i=2;
            }
            display.ShowTexture(n,i);
//            display.CopyFrameBuffer();
            display.Update();
        }
        //std::cin.ignore();
        t += 1.0;
        x = 0.01*t;
    }

    delete[] data;
}


int main()
{
    rotatingMonkeyLoop();
}
