#include "shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {
    ifstream vin, fin;
    string vertexString, fragmentString;
    //这两行确保ifstream能抛出异常
    vin.exceptions(ifstream::failbit | ifstream::badbit);
    fin.exceptions(ifstream::failbit | ifstream::badbit);
    try {
        vin.open(vertexShaderPath);
        fin.open(fragmentShaderPath);
        stringstream vstream, fstream;
        vstream << vin.rdbuf();
        fstream << fin.rdbuf();
        vertexString = vstream.str();
        fragmentString = fstream.str();
        vin.close();
        fin.close();
    } catch(std::ifstream::failure e) {
        cerr << "Fail to read shader file." << endl;
    }

    const char* vertexShaderCode = vertexString.c_str();
    const char* fragmentShaderCode = fragmentString.c_str();
    
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

void Shader::setFloat(const char* name, float value) {
    glUniform1f(glGetUniformLocation(shaderProgram, name), value);
}

void Shader::setInt(const char* name, int value) {
    glUniform1i(glGetUniformLocation(shaderProgram, name), value);
}


void Shader::setVec3(const char* name, glm::vec3 value) {
    glUniform3fv(glGetUniformLocation(shaderProgram, name), 1, glm::value_ptr(value));
}

void Shader::setMat4(const char* name, glm::mat4 value, int T) {
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name), 1, T, glm::value_ptr(value));
}