//
// Created by lx_tyin on 2022/11/25.
//

#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H

#include "Instance.h"

class Renderer{

    /**
     * recursive render instance.
     * \param cur current instance.
     * \param t Transform to global.
     */
    void render_instance(Instance *cur, Transform t);

public:

    /**
     * render all (in current_scene).
     */
    void render();
};


#endif //OPENGL_RENDERER_H
