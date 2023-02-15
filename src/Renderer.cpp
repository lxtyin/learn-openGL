//
// Created by lx_tyin on 2022/11/25.
//

#include "Renderer.h"
#include "Instance.h"
#include "Transform.h"
#include "Camera.h"
#include <iostream>
#include <queue>

Scene* Renderer::current_scene = nullptr;
Camera* Renderer::current_camera = nullptr;

void Renderer::render_instance(Instance *cur, mat4 t) {
    if(cur->mesh && cur->material){
        cur->material->use();
        cur->material->shader->setMat4("model", t);
        cur->material->shader->setMat4("view", camera->view());
        cur->material->shader->setMat4("projection", camera->projection);

        cur->mesh->draw();
    }
    Instance *child;
    for(int i = 0; (child = cur->get_child(i)) != nullptr; i++){
		render_instance(child, t * child->transform.matrix());
    }
}

void Renderer::render() {
    if(scene == nullptr){
        std::cerr << "Renderer: No scene selected." << std::endl;
        return;
    }
    if(camera == nullptr){
        std::cerr << "Renderer: No camera selected." << std::endl;
        return;
    }

	current_scene = scene;
	current_camera = camera;

    render_instance(scene, mat4(1));
}

Renderer::Renderer(Scene* s, Camera* c):
	scene(s), camera(c){}
