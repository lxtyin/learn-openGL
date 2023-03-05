#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Transform.h"
#include "Shader.h"
#include <set>
#include <string>

using glm::vec3;
using glm::vec4;
using std::string;
using std::set;

const int TYPE_DIRECTION = 0;
const int TYPE_POINT = 1;
const int TYPE_SPOT = 2;
const int TYPE_SURROUND = 3;

class Light {
public:
    vec3 emission;
    Light(vec3 emi);

    /**
     * add light to a shader(as uniform).
     * \param s target shader
     * \param name light name, such as light[i]
     */
    virtual void apply(Shader& s, const string& name) = 0;
};

//平行光 独立存在 和物体无关
class DirectionalLight : public Light {
public:
    vec3 direction; //照射方向

	DirectionalLight(vec3 emi, vec3 direction = vec3(0, -1, 0));

    void apply(Shader& s, const string& name) override;
};

//点光源 本身具有Transform
class PointLight : public Light {
public:
    Transform transform;

    PointLight(vec3 emi);
    void apply(Shader& s, const string& name) override;
};

//聚光 本身具有Transform
class SpotLight : public Light {
public:
    Transform transform;
    vec3 direction; //照射方向
    float angleIn, angleOut; //角度值

    SpotLight(vec3 emi, float agin, float agout, vec3 direction = vec3(0, -1, 0));
    void apply(Shader& s, const string& name) override;
};

class SurroundLight : public Light {
public:
    SurroundLight(vec3 emi);
    void apply(Shader& s, const string& name) override;
};