//
// Created by lx_tyin on 2023/3/5.
//

#ifndef OPENGL_PHONGMATERIAL_H
#define OPENGL_PHONGMATERIAL_H

#include "Material.h"

class PhongMaterial: public Material {
public:

    Texture *diffuse_map = nullptr;
    vec3 base_color = vec3(1, 0, 1);
    Texture *specular_map = nullptr;
    float spininess = 64;
    bool use_light = true;

    void update_shader() override;
    void use() override;
};


#endif //OPENGL_PHONGMATERIAL_H
