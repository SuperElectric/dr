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

void total_rewrite(){

    char* label ("DR_DIRECTORY");
    std::string DR_DIRECTORY (getenv(label));


    // The Display class only handles SDL stuff, and sets SDL settings in its constructor
    int width = 1024;
    int height = 1024;
    Display display(width,height, "");


    // Create a mesh from an OBJ file, and create a simple square for doing compositing
    // from textures which have been rendered to.


    // Create a texture, preferably a 2D array type, for texturing the loaded mesh.
    // Bind this texture to some texture unit, and to the appropriate sampler in
    // the fragment shader.
    // Generate all mip-map levels for this texture; make sure these are being sampled correctly


    // Create two shader programs, from files. The first is used for shading the mesh.
    // The second shader is used for compositing: namely calculating the motion derivatives
    // from the image of the mesh and the depth map.
    // Create a third shader program and corresponding vs and fs files for displaying
    // the render result on-screen, or in a differently scaled framebuffer
    // It is probably good to implement the three different types of shaders in three
    // different classes, each normally used only once.


    // Create a framebuffer, and create N textures and corresponding colour attachments
    // on the framebuffer (or alternatively create a single 2D-array type texture).
    // Also create a depth texture and the corresponding attachment
    // (but of course this could just be a colour texture instead)
    // bind these textures to appropriate unused texture units


    // Enable depth testing

    float t=0, x=0, c=0, s=0;
    while(!display.IsClosed())
    {
        // bind the offscreen frame buffer and, if a 2d-array type texture is not used,
        // activate only the image-itself attachment and the depth attachment for rendering.
        // Clear the depth and colour using glClear(...)
        // Bind the mesh-shader and draw the mesh.
        // Activate only the derivative attachments
        // Bind the compositing-shader
        // Detach the image and depth textures from the framebuffer, in order to use as textures
        //   (if this is really necessary, we can't really use a single 2d-array type texture for
        //    all the derivatives and the image)
        // Bind these textures to the appropriate texture units
        // Clear the actual depth buffer, but not the depth map texture
        // Update the compositing-shader with resolution info
        // Draw a square

        // Now detach all derivative textures from the framebuffer
        // Bind all these textures to appropriate texture units, and to appropriate samplers in
        // the displaying-fragment-shader.
        // Bind the displaying-fragment-shader
        // Update information such as display mode, and which derivative to display
        // Clear depth and colour
        // Draw a square



        // Note: make sure that the depth map passed to the compositing shader has the depth
        // of any "undrawn" pixels set to 1.0






        glClearColor(0.0,0.15,0.5,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        display.Update();
    }
    t += 1.0;
    x = 0.01*t;
}


int main()
{
    total_rewrite();
}
