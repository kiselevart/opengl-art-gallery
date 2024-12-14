#include "shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>

// Utility function to check OpenGL errors
void checkOpenGLError(const std::string &message) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL Error (" << message << "): " << err << std::endl;
    }
}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // Open files
    vShaderFile.open(vertexPath);
    if (!vShaderFile.is_open()) {
        std::cerr << "ERROR::SHADER::VERTEX::FILE_NOT_SUCCESFULLY_READ: " << vertexPath << std::endl;
    }
    fShaderFile.open(fragmentPath);
    if (!fShaderFile.is_open()) {
        std::cerr << "ERROR::SHADER::FRAGMENT::FILE_NOT_SUCCESFULLY_READ: " << fragmentPath << std::endl;
    }

    // Read file content into strings
    std::stringstream vShaderStream, fShaderStream;
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];

    // Compile vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    } else {
        std::cout << "Vertex Shader Compiled Successfully!" << std::endl;
    }

    // Compile fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    } else {
        std::cout << "Fragment Shader Compiled Successfully!" << std::endl;
    }

    // Link shaders into a program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    } else {
        std::cout << "Shader Program Linked Successfully!" << std::endl;
    }

    // Clean up shaders after linking
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    // Check for OpenGL errors after shader setup
    checkOpenGLError("Shader Compilation and Linking");
}

void Shader::use() {
    glUseProgram(ID);
    checkOpenGLError("glUseProgram");
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    checkOpenGLError("setMat4");
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    checkOpenGLError("setVec3");
}

void Shader::setFloat(const std::string &name, float value) {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    checkOpenGLError("setFloat");
}

void Shader::setInt(const std::string &name, int value) {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    checkOpenGLError("setInt");
}
