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

//void rotatingMonkeyLoop()
//{
//    char* label ("DR_DIRECTORY");
//    std::string DR_DIRECTORY (getenv(label));
//
//     //temporary white pixels
//        float* data = new float[10000000];
//        for (int i=0; i<10000000; i++){
//            data[i] = 1.0;
//        }
//
//
//    int width = 1024;
//    int height = 1024;
//    Display display(width,height, "Hello World!", 1, 4);
//    Mesh mesh ((DR_DIRECTORY + "/assets/monkey2.obj").c_str());
//    ParameterVector parameters(ParameterVector::DEFAULT);
//    Shader shader(DR_DIRECTORY + "/assets/basicShader");
//    shader.Bind();
//    Texture texture0 ((DR_DIRECTORY + "/assets/monkey_light_0.png").c_str());
//    texture0.Bind(0, "sampler_0");
//    Texture texture1 ((DR_DIRECTORY + "/assets/monkey_light_1.png").c_str());
//    texture1.Bind(1, "sampler_1");
//    Texture texture2 ((DR_DIRECTORY + "/assets/monkey_light_2.png").c_str());
//    texture2.Bind(2, "sampler_2");
//    Texture texture3 ((DR_DIRECTORY + "/assets/monkey_light_3.png").c_str());
//    texture3.Bind(3, "sampler_3");
//
//    float t=0, x=0, c=0, s=0;
//    while(!display.IsClosed())
//    {
//        shader.Bind();
//        c = glm::cos(x);
//        s = glm::sin(x);
//        float elev = 0.5;
//        parameters.cameraTransformMatrix = glm::mat4(1.0,0.0,0.0 ,0.0,
//                                                     0.0,glm::cos(elev), glm::sin(elev),0.0,
//                                                     0.0,-glm::sin(elev), glm::cos(elev) ,0.0,
//                                                     0.0,0.0,-6.0,1.0)
//                                         * glm::mat4( c ,0.0,-s ,0.0,
//                                                     0.0,1.0,0.0,0.0,
//                                                      s ,0.0, c ,0.0,
//                                                     0.0,1.0,0.0,1.0);
//        shader.Update(parameters);
//        display.SetFrameBuffer(0);
//        display.Clear(0.0f,0.0f,0.0f,1.0f);
//        texture0.Bind(0, "sampler_0");
//        texture1.Bind(1, "sampler_1");
//        texture2.Bind(2, "sampler_2");
//        texture3.Bind(3, "sampler_3");
//        mesh.Draw();
//
//        {
//            int i=0;
//            int n=0;
//            if ((int)t % 180 < 200)
//            {
//                //display.SetRenderBuffer(0,0);
//                i=0;
//            }
//            else if ((int)t % 180 < 120)
//            {
////                display.SetRenderBuffer(0,1);
//                i=1;
//            }
//            else
//            {
////                display.SetRenderBuffer(0,2);
//                i=2;
//            }
//            display.ShowTexture(n,i);
////            display.CopyFrameBuffer();
//            display.Update();
//        }
//        //std::cin.ignore();
//        t += 1.0;
//        x = 0.01*t;
//    }
//
//    delete[] data;
//}

void loop(){

    char* label ("DR_DIRECTORY");
    std::string DR_DIRECTORY (getenv(label));

    int width = 1024;
    int height = 1024;
    Display display(width,height, "");
    Mesh mesh((DR_DIRECTORY + "/assets/monkey2.obj").c_str());
    Mesh square;
    Texture surfaceColourTexture((DR_DIRECTORY + "/assets/monkey.png").c_str());
    Shader shadingShader((DR_DIRECTORY + "/assets/SHshader").c_str());
    Shader displayingShader((DR_DIRECTORY + "/assets/drawTexture").c_str());
    ParameterVector parameters(ParameterVector::DEFAULT);

    GLuint frameBuffer;
    GLuint imageBufferTexture;
    GLuint depthBuffer;
    glGenFramebuffers(1, &frameBuffer);
    glGenTextures(1, &imageBufferTexture);
    glGenRenderbuffers(1, &depthBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glBindTexture(GL_TEXTURE_2D, imageBufferTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, imageBufferTexture, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, depthBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    float t=0, x=0, c=0, s=0;
    while(!display.IsClosed())
    {
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


        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        GLenum renderBufferList[3] = {GL_COLOR_ATTACHMENT0};
        // remark: try swapping shadingShader.Bind() and glDrawBuffers(...) ?
        glDrawBuffers(1, renderBufferList);
        shadingShader.Bind();
        shadingShader.Update(parameters, true, width, height);
        surfaceColourTexture.Bind(0,"surface_colour_texture");
        glClearColor(0.5,0.2,0.2,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mesh.Draw();
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);   // uncomment if really necessary

//
//        {
//            GLenum renderBufferList[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
//            glDrawBuffers(3, renderBufferList);
//        }
//        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
//        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, 0, 0);
//        glActiveTexture(GL_TEXTURE1+0);
//        glBindTexture(GL_TEXTURE_2D, imageBufferTexture);
//        glUniform1i(glGetUniformLocation(shadingShader.gluint(), "first_pass_colour"), 1);
//        glActiveTexture(GL_TEXTURE1+1);
//        glBindTexture(GL_TEXTURE_2D, depthBufferTexture);
//        glUniform1i(glGetUniformLocation(shadingShader.gluint(), "first_pass_depth"), 2);
//        glActiveTexture(GL_TEXTURE0);
//        // Clear the actual depth buffer, but not the depth map texture
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        // Update the compositing-shader with resolution info
//        shadingShader.Update(parameters, false, width, height);
//        // Draw a square
//        square.Draw();


        displayingShader.Bind();
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, imageBufferTexture);
        glUniform1i(glGetUniformLocation(displayingShader.gluint(), "image"), 1);
        parameters.sphericalHarmonicCoefficients[0] = glm::vec3(t,t,t);
        displayingShader.Update(parameters, false, width, height);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //glViewport(0,0,width,height);
        glClearColor(0.0,0.15,0.5,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        square.Draw();

        display.Update();

    // uncomment if the line above that detaches imageBufferTexture from GL_COLOR_ATTACHMENT is also uncommented
//        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
//        //glBindTexture(GL_TEXTURE_2D, imageBufferTexture); // uncomment if really necessary
//        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, imageBufferTexture,0);

        t += 1.0;
        x = 0.01*t;
    }

    // delete stuff
    {
        GLuint textures[1] = {imageBufferTexture};//, depthBufferTexture, derivativesBufferTexture};
        glDeleteTextures(1, textures);
    }
    glDeleteRenderbuffers(1, &depthBuffer);
    glDeleteFramebuffers(1, &frameBuffer);
}


int main()
{
    loop();
}
