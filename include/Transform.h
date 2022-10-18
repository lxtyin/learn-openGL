#pragma once

#include "exglm.hpp"
#include "glad/glad.h"
#include "glfw/glfw3.h"

#define LOCAL_SPACE 0
#define WORLD_SPACE 1

// 表达一个变换、坐标系

class Transform {
public:
    glm::mat4 transmat;
    
    Transform();
    void translate(glm::vec3 v, int space_mode = LOCAL_SPACE);
    void translate(float vx, float vy, float vz, int space_mode = LOCAL_SPACE);
    void rotate(float angle, glm::vec3 v, int space_mode = LOCAL_SPACE);
    void rotate(float angle, float vx, float vy, float vz, int space_mode = LOCAL_SPACE);
    void scale(glm::vec3 v);
    void scale(float vx, float vy, float vz);

    glm::vec3 position();
    void setPosition(glm::vec3 v);
    void setPosition(float vx, float vy, float vz);
    void setLookAt(glm::vec3 tar, glm::vec3 vup); //z轴朝向tar，同时y轴设为vup
};


