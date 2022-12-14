//
// Created by lx_tyin on 2022/11/24.
//

#ifndef OPENGL_MATERIAL_H
#define OPENGL_MATERIAL_H

#include "tool/exglm.hpp"
#include "Texture.h"
#include "Shader.h"
#include "tool/tool.h"
#include <string>
#include <map>

using glm::vec3;
using std::string;

class Material{
public:
    std::map<string, Any> extend;     /** <stored extend valuea to be passed to shader. */

    Texture *diffuse_map = nullptr;
    vec3 diffuse_color = vec3(1, 0, 1);

    Texture *specular_map = nullptr;
    float spininess = 64;

    bool use_light = true;

    string shader_file = "../shader/standard";
    Shader *shader = nullptr;

    Material() = default;

    /**
     * recompile shader. shader should be recompiled if the material used different defines.
     */
    void update_shader();

    /**
     * apply this material.
     */
    void use();
};

#endif //OPENGL_MATERIAL_H
