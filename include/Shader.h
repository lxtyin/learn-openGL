#ifndef MAIN_CPP_SHADER_H
#define MAIN_CPP_SHADER_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include <string>
using std::string;

/* Shader类传入一个文件路径，以读取一个shader
 * 有关Shader，Light，Material协同工作的约定：
 * Shader文件中必要的三个uniform：model, view, projection
 * 如需应用光照，则还需要viewPos
 * 材质uniform规定名为 material.
 * 光照uniform规定名为 light[i].  光照数目 light_N，这些由光照类applyAllLightTo负责传入
 *
*/

class Shader {
    unsigned int shaderProgram;
public:
    Shader(const string &vertexShaderPath, const string &fragmentShaderSource);
    void use();
    void setFloat(const string &name, float value);
    void setInt(const string &name, int value);
    void setVec3(const string &name, glm::vec3 value);
    void setMat4(const string &name, glm::mat4 value, int T = GL_FALSE);
};

#endif
