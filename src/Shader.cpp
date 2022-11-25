#include "Shader.h"
#include <iostream>
using namespace std;

Shader::Shader(const string& vs_program, const string& fs_program) {
    const char* vertexShaderCode = vs_program.c_str();
    const char* fragmentShaderCode = fs_program.c_str();
    
    int success;
    char infoLog[512];
    //编译上述shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
    glCompileShader(vertexShader);
    //编译当然要检查是否成功，后面的也差不多
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cerr << "vertexShader compile failed: " << infoLog << endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cerr << "fragmentShader compile failed: " << infoLog << endl;
    }

    //连接，得到一个完整的shaderProgram
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //检查连接是否成功
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        cerr << "link failed: " << infoLog << endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use() {
    glUseProgram(shaderProgram);
}

void Shader::setAny(const string &name, const Any &value) {
    if(value.type_name == "int") {
        setInt(name, *(int*)value.value_ptr);
    } else if(value.type_name == "float") {
        setFloat(name, *(float*)value.value_ptr);
    } else if(value.type_name == "vec3") {
        setVec3(name, *(glm::vec3*)value.value_ptr);
    } else if(value.type_name == "mat4") {
        setMat4(name, *(glm::mat4*)value.value_ptr);
    }
}

void Shader::setFloat(const string& name, float value) {
    glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

void Shader::setInt(const string& name, int value) {
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

void Shader::setVec3(const string& name, glm::vec3 value) {
    glUniform3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setMat4(const string& name, glm::mat4 value, int T) {
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

