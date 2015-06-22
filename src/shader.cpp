#include "shader.h"
#include "mesh.h"
#include <iostream>
#include <fstream>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

static std::string LoadShader(const std::string& fileName);
static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
static GLuint CreateShader(const std::string& text, GLenum shaderType);

Shader::Shader(const std::string& fileName){
    m_program = glCreateProgram();
    m_shaders[0] = CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER);
    m_shaders[1] = CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);
    for (int i=0; i<NUM_SHADERS; i++){
        glAttachShader(m_program, m_shaders[i]);
    }
    glBindAttribLocation(m_program, Mesh::ATTRIBUTE_VERTEX_POSITIONS, "vertexPosition");
    glBindAttribLocation(m_program, Mesh::ATTRIBUTE_VERTEX_TEX_COORDS, "vertexTexCoord");
	glBindAttribLocation(m_program, Mesh::ATTRIBUTE_VERTEX_NORMALS, "vertexNormal");
    glLinkProgram(m_program);
    CheckShaderError(m_program, GL_LINK_STATUS, true, "Error: Program linking failed: ");
    glValidateProgram(m_program);
    CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "Error: Program is invalid: ");
    m_uniforms[U_CAMERA_4X4_TRANSFORM_MATRIX] = glGetUniformLocation(m_program, "cameraTransformMatrix");
    m_uniforms[U_CAMERA_3X3_ROTATION_MATRIX] = glGetUniformLocation(m_program, "cameraRotationMatrix");
    m_uniforms[U_CAMERA_4X4_PROJECTION_MATRIX] = glGetUniformLocation(m_program, "cameraProjectionMatrix");
    m_uniforms[U_LIGHT_POSITION] = glGetUniformLocation(m_program, "lightPosition");
    m_uniforms[U_LIGHT_COLOUR] = glGetUniformLocation(m_program, "lightColour");
    m_uniforms[U_MATERIAL_DIFFUSE_COLOUR] = glGetUniformLocation(m_program, "materialDiffuseColour");
    m_uniforms[U_MATERIAL_SPECULAR_COLOUR] = glGetUniformLocation(m_program, "materialSpecularColour");
    m_uniforms[U_MATERIAL_SPECULAR_HARDNESS] = glGetUniformLocation(m_program, "materialSpecularHardness");
}


void Shader::Update(ParameterVector& parameters){
    glUniformMatrix4fv(m_uniforms[U_CAMERA_4X4_TRANSFORM_MATRIX], 1, GL_FALSE,
                       &parameters.cameraTransformMatrix[0][0]);
    glUniformMatrix3fv(m_uniforms[U_CAMERA_3X3_ROTATION_MATRIX], 1, GL_FALSE,
                       &parameters.cameraRotationMatrix[0][0]);
    glUniformMatrix4fv(m_uniforms[U_CAMERA_4X4_PROJECTION_MATRIX], 1, GL_FALSE,
                       &parameters.cameraProjectionMatrix[0][0]);
    glUniform3fv(m_uniforms[U_LIGHT_POSITION], 1, &parameters.lightPosition[0]);
    glUniform3fv(m_uniforms[U_LIGHT_COLOUR], 1, &parameters.lightColour[0]);
    glUniform3fv(m_uniforms[U_MATERIAL_DIFFUSE_COLOUR], 1,
                 &parameters.materialDiffuseColour[0]);
    glUniform3fv(m_uniforms[U_MATERIAL_SPECULAR_COLOUR], 1,
                 &parameters.materialSpecularColour[0]);
    glUniform1fv(m_uniforms[U_MATERIAL_SPECULAR_HARDNESS], 1,
                 &parameters.materialSpecularHardness);
}


Shader::~Shader(){
    for (int i=0; i<NUM_SHADERS; i++){
        glDetachShader(m_program, m_shaders[i]);
        glDeleteShader(m_shaders[i]);
    }
    glDeleteProgram(m_program);
}


void Shader::Bind(){
    glUseProgram(m_program);
}

static GLuint CreateShader(const std::string& text, GLenum shaderType){
    GLuint shader = glCreateShader(shaderType);
    if (shader == 0){
        std::cerr << "Error: Shader creation failed!" << std::endl;
    }
    const GLchar* shaderSourceStrings[1];
    GLint shaderSourceStringLengths[1];
    shaderSourceStrings[0] = text.c_str();
    shaderSourceStringLengths[0] = text.length();
    glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths);
    glCompileShader(shader);
    CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error: Shader compilation failed: ");
    return shader;
}


static std::string LoadShader(const std::string& fileName){
    std::ifstream file;
    file.open((fileName).c_str());

    std::string output;
    std::string line;

    if(file.is_open())
    {
        while(file.good())
        {
            getline(file, line);
			output.append(line + "\n");
        }
    }
    else
    {
		std::cerr << "Unable to load shader: " << fileName << std::endl;
    }

    return output;
}

static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage){
    GLint success = 0;
    GLchar error[1024] = { 0 };

    if(isProgram)
        glGetProgramiv(shader, flag, &success);
    else
        glGetShaderiv(shader, flag, &success);

    if(success == GL_FALSE)
    {
        if(isProgram)
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        else
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);

        std::cerr << errorMessage << ": '" << error << "'" << std::endl;
    }
}
