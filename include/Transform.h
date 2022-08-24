#ifndef MAIN_CPP_TRANSFORM_H
#define MAIN_CPP_TRANSFORM_H

#include "glad/glad.h"
#include "glfw/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define MONE glm::mat4(1.0f)
#define LOCAL_SPACE 0
#define WORLD_SPACE 1

//pos和rot存储物体的旋转和位置矩阵
//pos为在其父坐标系下的位置，rot即自身坐标系，transform为 pos*rot
class Transform{
    glm::vec3 position;
    glm::mat4 rotation;
public:
    Transform();
    glm::mat4 transmat();
    void translate(glm::vec3 v, int space_mode = 0);
    void rotate(float angle, glm::vec3 v, int space_mode = 0);
    void translate(float vx, float vy, float vz, int space_mode = 0);
    void rotate(float angle, float vx, float vy, float vz, int space_mode = 0);
};

#endif //MAIN_CPP_TRANSFORM_H
