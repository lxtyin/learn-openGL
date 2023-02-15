//
// Created by lx_tyin on 2022/11/24.
//

#ifndef OPENGL_INSTANCE_H
#define OPENGL_INSTANCE_H

#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "Light.h"

class Instance {

    Instance *parent = nullptr;
    vector<Instance*> children;
public:
    Transform transform;                 /**< transform to parent. */
    Mesh *mesh = nullptr;                /**< mesh resource. */
    Material *material = nullptr;        /**< material resource. */

    Instance() = default;
    Instance(Instance *p);

	mat4 matrix_to_global();

    /**
     * get parent in instance-tree.
     * \return parent
     */
    Instance* get_parent();

    /**
     * get idx'th child in instance-tree.
     * \param idx
     * \return
     */
    Instance* get_child(int idx);

    /**
     * update parent, meanwhile update parent's children list.
     */
    void set_parent(Instance *p);

    /**
     * add instacne as a child, meanwhile update it's parent.
     * \param cd child instance
     * \return index of this child.
     */
    int add_child(Instance *cd);
};

class Scene : public Instance {
public:
    vector<Light*> lights;

    Scene() = default;

    /**
     * use a light in scene.
     * \param light light
     */
    void add_light(Light *light);
};

#endif //OPENGL_INSTANCE_H
