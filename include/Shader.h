#ifndef MAIN_CPP_SHADER_H
#define MAIN_CPP_SHADER_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "tool/tool.h"
#include "tool/Any.hpp"
#include <string>
using std::string;

class Shader {
    unsigned int shaderProgram;
public:
    Shader(const string &vs_program, const string &fs_program);
    void use();

    void setAny(const string &name, const Any &value);
    void setFloat(const string &name, float value);
    void setInt(const string &name, int value);
    void setVec2(const string &name, glm::vec2 value);
    void setVec3(const string &name, glm::vec3 value);
    void setMat4(const string &name, glm::mat4 value, int T = GL_FALSE);
};

#endif
