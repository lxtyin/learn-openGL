#include "Transform.h"
using namespace std;

//初始单位矩阵 和世界坐标重合
Transform::Transform() : transform(1.0f) {}

void Transform::translate(glm::vec3 v, int space_mode) {
    if(space_mode == LOCAL_SPACE) {
        transform = glm::translate(transform, v);
    } else if(space_mode == WORLD_SPACE) {
        transform = glm::translate(v, transform);
    }
}

void Transform::rotate(float angle, glm::vec3 v, int space_mode) {
    if(space_mode == LOCAL_SPACE) {
        transform = glm::rotate(transform, angle, v);
    } else if(space_mode == WORLD_SPACE) {
        transform = glm::rotate(angle, v, transform);
    }
}

void Transform::scale(glm::vec3 v) {
    transform = glm::scale(transform, v);
}

void Transform::translate(float vx, float vy, float vz, int space_mode){
    translate(glm::vec3(vx, vy, vz), space_mode);
}

void Transform::rotate(float angle, float vx, float vy, float vz, int space_mode){
    rotate(angle, glm::vec3(vx, vy, vz), space_mode);
}

void Transform::scale(float vx, float vy, float vz) {
    scale(glm::vec3(vx, vy, vz));
}