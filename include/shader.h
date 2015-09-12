#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <GL/glew.h>
#include "parameterVector.h"

std::string LoadShader(std::string fileName);
void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
GLuint CreateShader(const std::string& text, GLenum shaderType);

class Shader
{
    public:
        Shader(std::string fileName);
        void Bind();
        void Update(ParameterVector parameters, bool isFirstPass, int resolutionX, int resolutionY);
        GLint GetUniformLocation(char* name);
        virtual ~Shader();
        GLuint gluint(){
            return m_program;
        }
    protected:
    private:
        enum {NUM_SHADERS=2};
        enum {U_CAMERA_4X4_TRANSFORM_MATRIX,
              U_CAMERA_4X4_TRANSFORM_MATRIX_DERIVATIVE,
              U_CAMERA_3X3_ROTATION_MATRIX,
              U_CAMERA_4X4_PROJECTION_MATRIX,
              //U_LIGHT_POSITION,
              //U_LIGHT_COLOUR,
              //U_MATERIAL_DIFFUSE_COLOUR,
              //U_MATERIAL_SPECULAR_COLOUR,
              //U_MATERIAL_SPECULAR_HARDNESS,
              U_SPHERICAL_HARMONIC_COEFFICIENTS,
              U_IS_FIRST_PASS,
              U_RESOLUTION_X,
              U_RESOLUTION_Y,
              NUM_UNIFORMS};
        GLuint m_program;
        GLuint m_shaders[NUM_SHADERS];
        GLuint m_uniforms[NUM_UNIFORMS];
};

#endif // SHADER_H
