#include "parameterVector.h"

ParameterVector::ParameterVector(int e)
{
    if (e == ZEROES or e<0 or e>=NUM_CONSTRUCTORS)
    {
        cameraTransformMatrix = glm::mat4(0.0);
        cameraProjectionMatrix = glm::mat4(0.0);
        lightPosition = glm::vec3(0.0,0.0,0.0);
        lightColour = glm::vec3(0.0,0.0,0.0);
        materialDiffuseColour = glm::vec3(0.0,0.0,0.0);
        materialSpecularColour = glm::vec3(0.0,0.0,0.0);
        materialSpecularHardness = 0.0;
    }
    else if (e == DEFAULT)
    {
        cameraTransformMatrix = glm::mat4( 1 ,0.0, 0.0 ,0.0,
                                           0.0,1.0,0.0,0.0,
                                           0.0 ,0.0, 1.0 ,0.0,
                                           0.0,0.0,-2.0,1.0);
        float zNear = 0.1;
        float zFar = 100.0;
        cameraProjectionMatrix = glm::perspective(80.0f, 1.0f, zNear, zFar);
        lightPosition = glm::vec3(0.0,0.0,2.0);
        lightColour = glm::vec3(1.0,1.0,1.0);
        materialDiffuseColour = glm::vec3(0.5,0.5,0.5);
        materialSpecularColour = glm::vec3(1.0,1.0,1.0);
        materialSpecularHardness = 10.0;
    }
}

ParameterVector operator+(ParameterVector a, ParameterVector b)
{
    ParameterVector result(ParameterVector::ZEROES);
    result.cameraTransformMatrix = a.cameraTransformMatrix + b.cameraTransformMatrix;
    result.cameraProjectionMatrix = a.cameraProjectionMatrix + b.cameraProjectionMatrix;
    result.lightPosition = a.lightPosition + b.lightPosition;
    result.lightColour = a.lightColour + b.lightColour;
    result.materialDiffuseColour = a.materialDiffuseColour + b.materialDiffuseColour;
    result.materialSpecularColour = a.materialSpecularColour + b.materialSpecularColour;
    result.materialSpecularHardness = a.materialSpecularHardness + b.materialSpecularHardness;
    return result;
}

ParameterVector operator*(float a, ParameterVector b)
{
    ParameterVector result(ParameterVector::ZEROES);
    result.cameraTransformMatrix = a*b.cameraTransformMatrix;
    result.cameraProjectionMatrix = a*b.cameraProjectionMatrix;
    result.lightPosition = a*b.lightPosition;
    result.lightColour = a*b.lightColour;
    result.materialDiffuseColour = a*b.materialDiffuseColour;
    result.materialSpecularColour = a*b.materialSpecularColour;
    result.materialSpecularHardness = a*b.materialSpecularHardness;
    return result;
}

ParameterVector operator-(ParameterVector a, ParameterVector b)
{
    ParameterVector result(ParameterVector::ZEROES);
    result = a + -1.0*b;
    return result;
}
