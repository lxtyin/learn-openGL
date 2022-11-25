//
// Created by lx_tyin on 2022/11/24.
//

#include "tool.h"
#include "Material.h"
#include "Light.h"
#include "Instance.h"

void Material::update_shader(){
    string head = "#version 330 core\n";
    if(use_light)    head += "#define USE_LIGHT\n";
    if(diffuse_map)  head += "#define USE_DIFFUSE_MAP\n";
    if(specular_map) head += "#define USE_SPECULAR_MAP\n";

    string vs_program = head + read_file(shader_file + ".vs");
    string fs_program = head + read_file(shader_file + ".fs");
    shader = new Shader(vs_program, fs_program);
}

void Material::use(){
    if(!shader) update_shader();
    shader->use();

    int active_id = 0;
    if(diffuse_map){
        shader->setInt("diffuse_map", active_id);
        diffuse_map->activeTarget(active_id++);
    } else {
        shader->setVec3("diffuse_color", diffuse_color);
    }

    if(specular_map){
        shader->setFloat("spininess", spininess);
        shader->setInt("specular_map", active_id);
        specular_map->activeTarget(active_id++);
    }

    if(use_light){
        int n = 0;
        for(Light *light : Scene::current_scene->lights){
            light->apply(*shader, str_format("light[%d]", n++));
        }
        shader->setInt("light_N", n);
    }
}
