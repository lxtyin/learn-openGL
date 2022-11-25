#include "Transform.h"
using namespace std;

//初始单位矩阵 和世界坐标重合
Transform::Transform() : transmat(1.0f) {}

Transform::Transform(const mat4 &mt) : transmat(mt) {}

Transform::Transform(vec3 position, vec3 scale, vec3 rotation, const char *rule){
    transmat = mat4(1.0f);
    for(int i = 0;i < 3;i++){
        if(rule[i] == 'X'){
            transmat = glm::rotate(transmat, rotation.x, vec3(1, 0, 0)); // rotate in local space.
        } else if(rule[i] == 'Y'){
            transmat = glm::rotate(transmat, rotation.y, vec3(0, 1, 0));
        } else if(rule[i] == 'Z'){
            transmat = glm::rotate(transmat, rotation.z, vec3(0, 0, 1));
        }
    }
    transmat = glm::translate(position, transmat);  // move in world space
    transmat = glm::scale(transmat, scale);
}

void Transform::translate(glm::vec3 v, int space_mode) {
    if(space_mode == LOCAL_SPACE) {
        transmat = glm::translate(transmat, v);
    } else if(space_mode == PARENT_SPACE) {
        transmat = glm::translate(v, transmat);
    }
}

void Transform::rotate(float angle, glm::vec3 v, int space_mode) {
    if(space_mode == LOCAL_SPACE) {
        transmat = glm::rotate(transmat, angle, v);
    } else if(space_mode == PARENT_SPACE) {
        transmat = glm::rotate(angle, v, transmat);
    }
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

void Transform::setLookAt(glm::vec3 tar, glm::vec3 vup) {
    glm::vec3 pos = position();
    transmat = glm::lookAt(pos, tar, vup);
}

Transform Transform::operator*(const Transform &t) const{
    return Transform(transmat * t.transmat);
}