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
#include "gtest/gtest.h"

std::string dr_directory()
{
    char* label ("DR_DIRECTORY");
    std::string directory (getenv(label));
    return directory;
}
std::string DR_DIRECTORY = dr_directory();


void takePixelDifferences (float* pixels1, float* pixels0, float divisor,
                           int arraySize, float* outputPixels)
{
    for (int i=0; i<arraySize; i++)
    {
        outputPixels[i] = (pixels1[i] - pixels0[i]) / divisor;
    }
}

float comparePixels (float* pixels0, float* pixels1, int arraySize)
{
    float maxDifference = 0.0;
    for (int i=0; i<arraySize; i++)
    {
        maxDifference = std::max(maxDifference, glm::abs(pixels0[i] - pixels1[i]));
    }
    return maxDifference;
}


void numericalDerivative(Display& display, Mesh& mesh, Shader& shader,
                         ParameterVector& location, ParameterVector& direction,
                         float increment, float* pixels)
{
    int pixelArraySize =  display.render_width*display.render_height*3;
    float* pixels0 = new float[pixelArraySize];
    shader.Update(location);
    display.SetFrameBuffer(0);
    display.Clear(0.0f,0.0f,0.0f,1.0f);
    mesh.Draw();
    display.SetRenderBuffer(0,0);  // read from the image (non-derivative) renderbuffer
    display.SaveRenderBuffer(pixels0);

    float* pixels1 = new float[pixelArraySize];
    //ParameterVector newlocation = location;
    //newlocation.lightColour = location.lightColour + glm::vec3(increment,0.0,0.0);
    ParameterVector newlocation = location + increment*direction;
    shader.Update(newlocation);
    display.SetFrameBuffer(0);
    display.Clear(0.0f,0.0f,0.0f,1.0f);
    mesh.Draw();
    display.SetRenderBuffer(0,0);  // read from the image (non-derivative) renderbuffer
    display.SaveRenderBuffer(pixels1);

    //std::cout << "Maximum difference due to light colour change is: " << comparePixels(pixels1, pixels0, pixelArraySize) << std::endl;
    takePixelDifferences (pixels1, pixels0, increment, pixelArraySize, pixels);

    delete[] pixels0;
    delete[] pixels1;
}

void redDerivative(Display& display, Mesh& mesh, Shader& shader,
                   ParameterVector& location,
                   float* pixels)
{
    shader.Update(location);
    display.SetFrameBuffer(0);
    display.Clear(0.0f,0.0f,0.0f,1.0f);
    mesh.Draw();
    display.SetRenderBuffer(0,1);  // read from the red derivative renderbuffer
    display.SaveRenderBuffer(pixels);
}


float test(){
    int width = 1024;
    int height = 1024;
    Display display(width,height, "Test", 1, 2);
    Mesh mesh ((DR_DIRECTORY + "/assets/monkey.obj").c_str());
    ParameterVector parameters(ParameterVector::DEFAULT);
    Shader shader((DR_DIRECTORY + "/assets/basicShader").c_str());
    shader.Bind();

    ParameterVector directionRed (ParameterVector::ZEROES);
    directionRed.lightColour = glm::vec3(1.0,0.0,0.0);

    int pixelArraySize =  display.render_width*display.render_height*3;
    float* analyticDerivativePixels = new float[pixelArraySize];
    float* numericalDerivativePixels = new float[pixelArraySize];
    redDerivative(display,mesh,shader,parameters,analyticDerivativePixels);
    numericalDerivative(display,mesh,shader,parameters,directionRed,0.001,numericalDerivativePixels);
    float error = comparePixels(analyticDerivativePixels,
                                numericalDerivativePixels,
                                pixelArraySize);
    delete[] analyticDerivativePixels;
    delete[] numericalDerivativePixels;

    return error;
}


TEST(DerivativeTest, Red)
{
    std::cout << test() << std::endl;
    ASSERT_GE(0.001, test());
}


//int main(){
//    std::cout << test() << std::endl;
//}
