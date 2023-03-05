//
// Created by lx_tyin on 2022/11/24.
//

#ifndef OPENGL_MATERIAL_H
#define OPENGL_MATERIAL_H

#include "../tool/exglm.hpp"
#include "../Texture.h"
#include "../Shader.h"
#include "../tool/tool.h"
#include <string>
#include <map>

using glm::vec3;
using std::string;

class Material{
public:
    Shader *shader = nullptr;
    std::map<string, Any> extend;     /** <stored extend valuea to be passed to shader. */

    /**
     * recompile shader. shader should be recompiled if the material used different defines.
     */
    virtual void update_shader() = 0;

    /**
     * apply this material.
     */
    virtual void use() = 0;
};

#endif //OPENGL_MATERIAL_H
