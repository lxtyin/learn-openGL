//
// Created by lx_tyin on 2022/11/24.
//

#include "Instance.h"

Instance::Instance(Instance *p) {
    parent = p;
    p->children.push_back(p);
}

Transform Instance::transform_global() {
    Transform res = transform;
    Instance *ptr = parent;
    while(ptr != nullptr){
        res = ptr->transform * res;
        ptr = ptr->parent;
    }
    return res;
}

Instance *Instance::get_parent() {
    return parent;
}

Instance *Instance::get_child(int idx) {
    if(idx >= children.size()) return nullptr;
    return children[idx];
}

void Instance::set_parent(Instance *p) {
    if(parent != nullptr){
        for(auto it = parent->children.begin();it != parent->children.end();it++){
            if(*it == this){
                parent->children.erase(it);
                break;
            }
        }
    }
    parent = p;
    p->children.push_back(this);
}

int Instance::add_child(Instance *cd) {
    cd->set_parent(this);
    return (int)children.size() - 1;
}

Scene* Scene::current_scene = nullptr;

void Scene::use(){
    current_scene = this;
}

void Scene::add_light(Light *light){
    lights.push_back(light);
}