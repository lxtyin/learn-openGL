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

const int TYPE_PARALLEL = 0;
const int TYPE_POINT = 1;
const int TYPE_SPOT = 2;
const int TYPE_SURROUND = 3;
const vec3 DEFAULT_DECAY(1.0f, 0.09f, 0.032f); //默认衰减

class Light {
public:
    vec3 color;
    Light(vec3 c);

    /**
     * add light to a shader(as uniform).
     * \param s target shader
     * \param name light name, such as light[i]
     */
    virtual void apply(Shader& s, const string& name) = 0;
};

//平行光 独立存在 和物体无关
class ParallelLight : public Light {
public:
    vec3 direction; //照射方向
    ParallelLight(vec3 c, vec3 direction = vec3(0, -1, 0));
    void apply(Shader& s, const string& name) override;
};

//点光源 本身具有Transform
class PointLight : public Light {
public:
    Transform transform;
    vec3 decay;
    
    PointLight(vec3 c);
    void apply(Shader& s, const string& name) override;
};

//聚光 本身具有Transform
class SpotLight : public Light {
public:
    Transform transform;
    vec3 decay;
    vec3 direction; //照射方向
    float angleIn, angleOut; //角度值
    
    SpotLight(vec3 c, float agin, float agout, vec3 direction = vec3(0, -1, 0));
    void apply(Shader& s, const string& name) override;
};

class SurroundLight : public Light {
public:
    SurroundLight(vec3 c);
    void apply(Shader& s, const string& name) override;
};