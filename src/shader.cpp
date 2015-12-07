#include "shader.h"
#include "mesh.h"
#include <iostream>
#include <fstream>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

Shader::Shader(std::string fileName){
    m_program = glCreateProgram();
    m_shaders[0] = CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER);
    m_shaders[1] = CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);
    for (int i=0; i<NUM_SHADERS; i++){
        glAttachShader(m_program, m_shaders[i]);
    }
    glBindAttribLocation(m_program, Mesh::ATTRIBUTE_VERTEX_POSITIONS, "vertex_position");
    glBindAttribLocation(m_program, Mesh::ATTRIBUTE_VERTEX_TEX_COORDS, "vertex_tex_coord");
	glBindAttribLocation(m_program, Mesh::ATTRIBUTE_VERTEX_NORMALS, "vertex_normal");
    glLinkProgram(m_program);
    CheckShaderError(m_program, GL_LINK_STATUS, true, "Error: Program linking failed: ");
    glValidateProgram(m_program);
    CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "Error: Program is invalid: ");
    m_uniforms[U_CAMERA_4X4_TRANSFORM_MATRIX] = glGetUniformLocation(m_program, "camera_transform_matrix");
//    m_uniforms[U_CAMERA_4X4_TRANSFORM_MATRIX_DERIVATIVE] = glGetUniformLocation(m_program, "camera_transform_matrix_derivative");
    m_uniforms[U_CAMERA_3X3_ROTATION_MATRIX] = glGetUniformLocation(m_program, "camera_rotation_matrix");
    m_uniforms[U_CAMERA_4X4_PROJECTION_MATRIX] = glGetUniformLocation(m_program, "camera_projection_matrix");
//    m_uniforms[U_LIGHT_POSITION] = glGetUniformLocation(m_program, "lightPosition");
//    m_uniforms[U_LIGHT_COLOUR] = glGetUniformLocation(m_program, "lightColour");
//    m_uniforms[U_MATERIAL_DIFFUSE_COLOUR] = glGetUniformLocation(m_program, "materialDiffuseColour");
//    m_uniforms[U_MATERIAL_SPECULAR_COLOUR] = glGetUniformLocation(m_program, "materialSpecularColour");
//    m_uniforms[U_MATERIAL_SPECULAR_HARDNESS] = glGetUniformLocation(m_program, "materialSpecularHardness");
    m_uniforms[U_SPHERICAL_HARMONIC_COEFFICIENTS] = glGetUniformLocation(m_program, "spherical_harmonic_coefficients");
    m_uniforms[U_IS_FIRST_PASS] = glGetUniformLocation(m_program, "is_first_pass");
    m_uniforms[U_RESOLUTION_X] = glGetUniformLocation(m_program, "resolution_x");
    m_uniforms[U_RESOLUTION_Y] = glGetUniformLocation(m_program, "resolution_y");
    //glBindFragDataLocation(m_program, 0, "outputColour0");
    //glBindFragDataLocation(m_program, 1, "outputColour1");
}


void Shader::Update(ParameterVector parameters, bool isFirstPass, int resolutionX, int resolutionY){

    glUniformMatrix4fv(m_uniforms[U_CAMERA_4X4_TRANSFORM_MATRIX], 1, GL_FALSE, &parameters.cameraTransformMatrix[0][0]);
    //glUniformMatrix4fv(m_uniforms[U_CAMERA_4X4_TRANSFORM_MATRIX_DERIVATIVE], 1, GL_FALSE, &parameters.cameraTransformMatrixDerivative[0][0]);
    glUniformMatrix4fv(m_uniforms[U_CAMERA_4X4_PROJECTION_MATRIX], 1, GL_FALSE, &parameters.cameraProjectionMatrix[0][0]);
    glUniform3fv(m_uniforms[U_SPHERICAL_HARMONIC_COEFFICIENTS], 9, &parameters.sphericalHarmonicCoefficients[0][0]);
    glUniform1i(m_uniforms[U_IS_FIRST_PASS], isFirstPass);
    glUniform1i(m_uniforms[U_RESOLUTION_X], resolutionX);
    glUniform1i(m_uniforms[U_RESOLUTION_Y], resolutionY);
//    glUniform3fv(m_uniforms[U_LIGHT_POSITION], 1, &parameters.lightPosition[0]);
//    glUniform3fv(m_uniforms[U_LIGHT_COLOUR], 1, &parameters.lightColour[0]);
//    glUniform3fv(m_uniforms[U_MATERIAL_DIFFUSE_COLOUR], 1,
//                 &parameters.materialDiffuseColour[0]);
//    glUniform3fv(m_uniforms[U_MATERIAL_SPECULAR_COLOUR], 1,
//                 &parameters.materialSpecularColour[0]);
//    glUniform1fv(m_uniforms[U_MATERIAL_SPECULAR_HARDNESS], 1,
//                 &parameters.materialSpecularHardness);
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

GLint Shader::GetUniformLocation(char* name){
    return glGetUniformLocation(m_program, name);
}



GLuint CreateShader(const std::string& text, GLenum shaderType){
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


std::string LoadShader(std::string fileName){
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

void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage){
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
