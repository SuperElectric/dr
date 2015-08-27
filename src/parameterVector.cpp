#include "parameterVector.h"

ParameterVector::ParameterVector(int e)
{
    if (e == ZEROES or e<0 or e>=NUM_CONSTRUCTORS)
    {
        cameraTransformMatrix = glm::mat4(0.0);
        cameraProjectionMatrix = glm::mat4(0.0);
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
    }

    for (int i=0; i<9; i++){
        sphericalHarmonicCoefficients[i] = 0.0;
    }
}

ParameterVector operator+(ParameterVector a, ParameterVector b)
{
    ParameterVector result(ParameterVector::ZEROES);
    result.cameraTransformMatrix = a.cameraTransformMatrix + b.cameraTransformMatrix;
    result.cameraProjectionMatrix = a.cameraProjectionMatrix + b.cameraProjectionMatrix;
    for (int i=0; i<9; i++){
        result.sphericalHarmonicCoefficients[i] = a.sphericalHarmonicCoefficients[i] + b.sphericalHarmonicCoefficients[i];
    }
    return result;
}

ParameterVector operator*(float a, ParameterVector b)
{
    ParameterVector result(ParameterVector::ZEROES);
    result.cameraTransformMatrix = a*b.cameraTransformMatrix;
    result.cameraProjectionMatrix = a*b.cameraProjectionMatrix;
    for (int i=0; i<9; i++){
        result.sphericalHarmonicCoefficients[i] = a*b.sphericalHarmonicCoefficients[i];
    }
    return result;
}

ParameterVector operator-(ParameterVector a, ParameterVector b)
{
    ParameterVector result(ParameterVector::ZEROES);
    result = a + -1.0*b;
    return result;
}
