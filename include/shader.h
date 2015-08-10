#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <GL/glew.h>
#include "parameterVector.h"


class Shader
{
    public:
        Shader(const std::string& fileName);
        void Bind();
        void Update(float t);
        void Update(ParameterVector parameters);
        GLint GetUniformLocation(char* name);
        virtual ~Shader();
    protected:
    private:
        enum {NUM_SHADERS=2};
        enum {U_CAMERA_4X4_TRANSFORM_MATRIX,
              U_CAMERA_3X3_ROTATION_MATRIX,
              U_CAMERA_4X4_PROJECTION_MATRIX,
              U_LIGHT_POSITION,
              U_LIGHT_COLOUR,
              U_MATERIAL_DIFFUSE_COLOUR,
              U_MATERIAL_SPECULAR_COLOUR,
              U_MATERIAL_SPECULAR_HARDNESS,
              NUM_UNIFORMS};
        GLuint m_program;
        GLuint m_shaders[NUM_SHADERS];
        GLuint m_uniforms[NUM_UNIFORMS];
};
#endif // SHADER_H
