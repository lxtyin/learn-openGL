#include "Transform.h"
using namespace std;

Transform::Transform(): position(0, 0, 0), rotation(MONE){}

//返回物体局部坐标转化到其父坐标的矩阵
glm::mat4 Transform::transmat(){
    return glm::translate(MONE, position) * rotation;
}

void Transform::translate(glm::vec3 v, int space_mode){
    if(space_mode == LOCAL_SPACE){
        position += glm::vec3(rotation * glm::vec4(v, 1));
//        glm::translate(rotation, v) * glm::inverse(rotation) * glm::vec4(position, 1);
    }else if(space_mode == WORLD_SPACE){
        position += v;
    }
}

void Transform::rotate(float angle, glm::vec3 v, int space_mode){
    if(space_mode == LOCAL_SPACE){
        rotation = glm::rotate(rotation, angle, v);
    }else if(space_mode == WORLD_SPACE){
        //这里比较特殊，按照逻辑，“绕世界坐标旋转”意味着以(0,0,0)点为中心公转
        //而这里一般希望表达的是：以自身为中心，轴向同世界坐标地旋转
        //所以我们把轴变化到自身坐标系下变换
        glm::mat4 rot = glm::rotate(MONE, angle, v);
        rotation = rot * rotation;
    }
}

void Transform::translate(float vx, float vy, float vz, int space_mode){
    translate(glm::vec3(vx, vy, vz), space_mode);
}

void Transform::rotate(float angle, float vx, float vy, float vz, int space_mode){
    rotate(angle, glm::vec3(vx, vy, vz), space_mode);
}