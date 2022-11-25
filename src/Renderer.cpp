//
// Created by lx_tyin on 2022/11/25.
//

#include "Renderer.h"
#include "Instance.h"
#include "Transform.h"
#include "Camera.h"
#include <iostream>
#include <queue>

void Renderer::render_instance(Instance *cur, Transform t) {
    if(cur->mesh && cur->material){
        cur->material->use();
        cur->material->shader->setMat4("model", t.transmat);
        cur->material->shader->setMat4("view", Camera::current_camera->view.transmat);
        cur->material->shader->setMat4("projection", Camera::current_camera->projection);

        cur->mesh->draw();
    }
    Instance *child;
    for(int i = 0; (child = cur->get_child(i)) != nullptr; i++){
        render_instance(child, t * child->transform);
    }
}

void Renderer::render() {
    if(Scene::current_scene == nullptr){
        std::cerr << "Renderer: No scene selected." << std::endl;
        return;
    }
    if(Camera::current_camera == nullptr){
        std::cerr << "Renderer: No camera selected." << std::endl;
        return;
    }
    render_instance(Scene::current_scene, Transform());
}