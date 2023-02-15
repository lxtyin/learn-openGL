//
// Created by lx_tyin on 2022/11/25.
//

#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H

#include "Instance.h"
#include "Camera.h"

class Renderer{

    /**
     * recursive render instance.
     * \param cur current instance.
     * \param t Transform to global.
     */
    void render_instance(Instance *cur, mat4 t);

public:

	static Scene* current_scene;			/** < is setted before render */
	static Camera* current_camera;			/** < is setted before render */

	Scene *scene = nullptr;
	Camera *camera = nullptr;

	Renderer(Scene *s, Camera *c);

    /**
     * render all.
     */
    void render();
};


#endif //OPENGL_RENDERER_H
