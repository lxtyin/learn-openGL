//
// Created by lx_tyin on 2023/3/5.
//

#include "DisneyMaterial.h"
#include "../Light.h"
#include "../Instance.h"
#include "../Renderer.h"


void DisneyMaterial::update_shader() {
    string head = "#version 330 core\n";
    if(diffuse_map)  head += "#define USE_DIFFUSE_MAP\n";

    string vs_program = head + read_file("shader/standard.vert");
    string fs_program = head + read_file("shader/disney.frag");
    shader = new Shader(vs_program, fs_program);
}

void DisneyMaterial::use() {
    if(!shader) update_shader();
    shader->use();

    int active_id = 0;
    if(diffuse_map){
        shader->setInt("diffuse_map", active_id);
        diffuse_map->activeTarget(active_id++);
    }

    shader->setVec3("base_color", base_color);
    shader->setFloat("roughness", roughness);
    shader->setFloat("metallic", metallic);
    shader->setFloat("specular", specular);
    shader->setFloat("specular_tint", specular_tint);
    shader->setFloat("sheen", sheen);
    shader->setFloat("sheenTint", sheenTint);
    shader->setFloat("subsurface", subsurface);
    shader->setFloat("clearcoat", clearcoat);
    shader->setFloat("clearcoat_gloss", clearcoat_gloss);
    shader->setFloat("anisotropic", anisotropic);

    int n = 0;
    for(Light *light : Renderer::current_scene->lights){
        light->apply(*shader, str_format("light[%d]", n++));
    }
    shader->setInt("light_N", n);

    for(auto &[name, value] : extend){
        shader->setAny(name, value);
    }

}
