#pragma once

#include "tool/exglm.hpp"
#include "glad/glad.h"
#include "glfw/glfw3.h"

#define LOCAL_SPACE 0
#define PARENT_SPACE 1

using glm::vec3;
using glm::mat4;

class Transform {
public:
    mat4 transmat;
    
    Transform();

    Transform(const mat4 &mt);

    /**
     * initialize transform by position, rotation and rule.
     * \param rotation euler angle. use dynamic euler angle.
     * \param rule as euler rotation order.
     */
    Transform(vec3 position, vec3 scale, vec3 rotation, const char *rule = "YXZ");

    void translate(vec3 v, int space_mode = LOCAL_SPACE);
    void rotate(float angle, vec3 v, int space_mode = LOCAL_SPACE);
    void scale(vec3 v);

    vec3 position();
    void setPosition(vec3 v);
    void setLookAt(vec3 tar, vec3 vup); //z轴朝向tar，同时y轴设为vup

    Transform operator *(const Transform &t)const;
};


