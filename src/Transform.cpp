#include "Transform.h"
using namespace std;

//初始单位矩阵 和世界坐标重合
Transform::Transform() : transmat(1.0f) {}

void Transform::translate(glm::vec3 v, int space_mode) {
    if(space_mode == LOCAL_SPACE) {
        transmat = glm::translate(transmat, v);
    } else if(space_mode == WORLD_SPACE) {
        transmat = glm::translate(v, transmat);
    }
}
void Transform::translate(float vx, float vy, float vz, int space_mode) {
    translate(glm::vec3(vx, vy, vz), space_mode);
}

void Transform::rotate(float angle, glm::vec3 v, int space_mode) {
    if(space_mode == LOCAL_SPACE) {
        transmat = glm::rotate(transmat, angle, v);
    } else if(space_mode == WORLD_SPACE) {
        transmat = glm::rotate(angle, v, transmat);
    }
}
void Transform::rotate(float angle, float vx, float vy, float vz, int space_mode) {
    rotate(angle, glm::vec3(vx, vy, vz), space_mode);
}

void Transform::scale(float vx, float vy, float vz) {
    scale(glm::vec3(vx, vy, vz));
}
void Transform::scale(glm::vec3 v) {
    transmat = glm::scale(transmat, v);
}

glm::vec3 Transform::position() {
    return transmat[3];
}

void Transform::setPosition(glm::vec3 v) {
    transmat[3] = glm::vec4(v, 1.0);
}
void Transform::setPosition(float vx, float vy, float vz) {
    setPosition(glm::vec3(vx, vy, vz));
}