//
// Created by lx_tyin on 2022/11/25.
//

#ifndef OPENGL_CAMERA_H
#define OPENGL_CAMERA_H

#include "Transform.h"
#include "Instance.h"

class Camera: public Instance{
public:
    mat4 projection;

    static Camera* create_perspective_camera(float fov, float aspect, float zNear, float zFar);
//    static Camera* create_ortho_camera();

	/**
	 * get view matrix.
	 * \return view matrix.
	 */
	mat4 view();
};


#endif //OPENGL_CAMERA_H
