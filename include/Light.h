#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Transform.h"
#include "Shader.h"
#include <string>

using glm::vec3;
using glm::vec4;
using std::string;

const int TYPE_PARALLEL = 0;
const int TYPE_POINT = 1;
const int TYPE_SPOT = 2;
const float DEFAULT_AMBIENT = 0.2; //默认环境光配比，构造时使用
const float DEFAULT_DIFFUSE = 0.6; //默认漫反射光配比，构造时使用
const vec3 DEFAULT_DECAY(1.0f, 0.09f, 0.032f); //默认衰减

class Light {
public:
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    Light(vec3 color);
    void setAmbientStrength(float r);
    void setDiffuseStrength(float r);
    virtual void apply(Shader& s, const string &target) = 0;
};

//平行光 独立存在 和物体无关
class ParallelLight : public Light {
public:
    vec3 direction; //照射方向
    ParallelLight(vec3 color, vec3 direction = vec3(0, -1, 0));
    void apply(Shader& s, const string& target) override;
};

//点光源 本身作为一个物体具有Transform
class PointLight : public Light {
public:
    Transform transform;
    vec3 decay;
    
    PointLight(vec3 color);
    void apply(Shader& s, const string& target) override;
};

//聚光 本身作为一个物体具有Transform
class SpotLight : public Light {
public:
    Transform transform;
    vec3 decay;
    vec3 direction; //照射方向
    float angleIn, angleOut; //角度值
    
    SpotLight(vec3 color, float agin, float agout, vec3 direction = vec3(0, -1, 0));
    void apply(Shader& s, const string& target) override;
};
