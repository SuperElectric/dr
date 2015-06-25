#ifndef PARAMETERVECTOR_H
#define PARAMETERVECTOR_H
#include "glm/glm.hpp"
//#include "glm/gtx/transform.hpp"
#include "GL/glew.h"

class ParameterVector
{
    public:
    ParameterVector(){};
    glm::mat4 cameraTransformMatrix;
    glm::mat3 cameraRotationMatrix;
    glm::mat4 cameraProjectionMatrix;
    glm::vec3 lightPosition;
    glm::vec3 lightColour;
    glm::vec3 materialDiffuseColour;
    glm::vec3 materialSpecularColour;
    GLfloat materialSpecularHardness;
};

#endif
