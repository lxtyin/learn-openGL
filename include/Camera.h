//
// Created by lx_tyin on 2022/11/25.
//

#ifndef OPENGL_CAMERA_H
#define OPENGL_CAMERA_H

#include "Transform.h"

class Camera{
public:
    Transform view;
    mat4 projection;

    static Camera* current_camera;

    static Camera* create_perspective_camera(float fov, float aspect, float zNear, float zFar);
//    static Camera* create_ortho_camera();

    /**
     * switch to this camera.
     */
    void use();
};


#endif //OPENGL_CAMERA_H
