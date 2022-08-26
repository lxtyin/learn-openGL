#ifndef MAIN_CPP_SHADER_H
#define MAIN_CPP_SHADER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glad/glad.h"
#include "glfw/glfw3.h"

class Shader{
    unsigned int shaderProgram;
public:
    Shader(const char *vertexShaderPath, const char *fragmentShaderSource);
    void use();
    void setFloat(const char *name, float value);
    void setInt(const char* name, int value);
    void setVec3(const char* name, glm::vec3 value);
    void setMat4(const char* name, glm::mat4 value, int T = GL_FALSE);
};

#endif
