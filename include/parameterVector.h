#ifndef PARAMETERVECTOR_H
#define PARAMETERVECTOR_H
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "GL/glew.h"

class ParameterVector
{
public:

    enum {ZEROES, DEFAULT, NUM_CONSTRUCTORS};

    ParameterVector(int e);
    glm::mat4 cameraTransformMatrix;
    //glm::mat4 cameraTransformMatrixDerivative;
    glm::mat4 cameraProjectionMatrix;
    glm::vec3 sphericalHarmonicCoefficients[9];
};

ParameterVector operator+(ParameterVector a, ParameterVector b);
ParameterVector operator*(float a, ParameterVector b);
ParameterVector operator-(ParameterVector a, ParameterVector b);

#endif
