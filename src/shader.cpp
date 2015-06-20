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
    glBindAttribLocation(m_program, Mesh::XYZ_ATTRIBUTE, "position");
    glBindAttribLocation(m_program, Mesh::UV_ATTRIBUTE, "texCoord");
	glBindAttribLocation(m_program, Mesh::NORMALS_ATTRIBUTE, "normal");
    glLinkProgram(m_program);
    CheckShaderError(m_program, GL_LINK_STATUS, true, "Error: Program linking failed: ");
    glValidateProgram(m_program);
    CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "Error: Program is invalid: ");
    m_uniforms[U_TRANSFORM_4x4_MATRIX] = glGetUniformLocation(m_program, "transformMatrix");
    m_uniforms[U_TRANSFORM_3X3_ROTATION_MATRIX] = glGetUniformLocation(m_program, "rotationMatrix");
    m_uniforms[U_CAMERA_MATRIX] = glGetUniformLocation(m_program, "cameraMatrix");
    m_uniforms[U_LIGHT_POSITION] = glGetUniformLocation(m_program, "lightPosition");
}

void Shader::Update(float t){
    // temporary hard coded values
    float c = glm::cos(t);
    float s = glm::sin(t);
    // elements listed in column major order, so matrix is transpose of one below
    glm::mat4 transformMatrix = glm::mat4(  c,0.0, -s,0.0,
                                          0.0,1.0,0.0,0.0,
                                            s,0.0,  c,0.0,
                                          0.0,0.0,-2.0,1.0);

    glm::mat3 rotationMatrix = glm::mat3(transformMatrix);
    float zNear = 0.1;
    float zFar = 100.0;
    //glm::mat4 cameraMatrix = glm::perspective(80.0f, 1.0f, zNear, zFar);
    glm::mat4 cameraMatrix = glm::mat4(1.0,0.0,0.0,0.0,
                                          0.0,1.0,0.0,0.0,
                                          0.0,0.0, -(zFar+zNear)/(zFar-zNear),-1.0,
                                          0.0,0.0,-2.0*zFar*zNear/(zFar-zNear),0.0);

    //glm::mat4 cameraMatrix = glm::mat4();
    glm::vec3 lightPosition = glm::vec3(0.0,0.0,2.0);

    glUniformMatrix4fv(m_uniforms[U_TRANSFORM_4x4_MATRIX], 1, GL_FALSE, &transformMatrix[0][0]);
    glUniformMatrix3fv(m_uniforms[U_TRANSFORM_3X3_ROTATION_MATRIX], 1, GL_FALSE, &rotationMatrix[0][0]);
    glUniformMatrix4fv(m_uniforms[U_CAMERA_MATRIX], 1, GL_FALSE, &cameraMatrix[0][0]);
    glUniform3fv(m_uniforms[U_LIGHT_POSITION], 1, &lightPosition[0]);
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
