//
// Created by lx_tyin on 2023/3/5.
//

#ifndef OPENGL_DISNEYMATERIAL_H
#define OPENGL_DISNEYMATERIAL_H

#include "Material.h"

class DisneyMaterial: public Material {

public:

    Texture *diffuse_map = nullptr;
    vec3 base_color;
    float metallic = 0.5;
    float roughness = 0.5;
    float specular = 0;
    float specular_tint = 0;
    float sheen = 0;
    float sheenTint = 0;
    float subsurface = 0;
    float clearcoat = 0;
    float clearcoat_gloss = 0;
    float anisotropic = 0;

    void update_shader() override;
    void use() override;

};


#endif //OPENGL_DISNEYMATERIAL_H
