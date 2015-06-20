#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <GL/glew.h>


class Shader
{
    public:
        Shader(const std::string& fileName);
        void Bind();
        void Update(float t);
        virtual ~Shader();
    protected:
    private:
        enum {NUM_SHADERS=2};
        enum {U_TRANSFORM_4x4_MATRIX,
              U_TRANSFORM_3X3_ROTATION_MATRIX,
              U_CAMERA_MATRIX,
              U_LIGHT_POSITION,
              NUM_UNIFORMS};
        GLuint m_program;
        GLuint m_shaders[NUM_SHADERS];
        GLuint m_uniforms[NUM_UNIFORMS];
};
#endif // SHADER_H
