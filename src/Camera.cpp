//
// Created by lx_tyin on 2022/11/25.
//

#include "Camera.h"

Camera* Camera::current_camera = nullptr;

Camera* Camera::create_perspective_camera(float fov, float aspect, float zNear, float zFar){
    Camera *res = new Camera;
    res->projection = glm::perspective(fov, aspect, zNear, zFar);
    return res;
}

void Camera::use(){
    current_camera = this;
}

