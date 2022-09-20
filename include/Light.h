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
const float DEFAULT_AMBIENT = 0.2; //默认环境光配比，构造时使用
const float DEFAULT_DIFFUSE = 0.6; //默认漫反射光配比，构造时使用
const vec3 DEFAULT_DECAY(1.0f, 0.09f, 0.032f); //默认衰减

/*
光的环境光颜色和漫反射颜色均为镜面光*一定系数，镜面光颜色即光本身颜色。
对于特殊需求 需自行更改颜色

光照类与shader需就命名达成约定，shader中的光源类：
struct Light{
    int type;
    vec3 direction;
    vec3 position;
    vec3 decay;
    float inCut, outCut;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light[16];
uniform int light_N;
*/

class Light {
    //暂时单一场景 此静态变量存储场景中所有光源
    static set<Light*> all_lights;
public:
    //按照规范设置所有光照，要求shader中命名: light_N, light[]
    static void applyAllLightTo(Shader& s);

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    Light(vec3 color);
    ~Light();
    void setAmbientStrength(float r);
    void setDiffuseStrength(float r);
    //target为shader中的接受此光照信息的对象名
    virtual void apply(Shader& s, const string& target) = 0;
};

//平行光 独立存在 和物体无关
class ParallelLight : public Light {
public:
    vec3 direction; //照射方向
    ParallelLight(vec3 color, vec3 direction = vec3(0, -1, 0));
    void apply(Shader& s, const string& target) override;
};

//点光源 本身具有Transform
class PointLight : public Light {
public:
    Transform transform;
    vec3 decay;
    
    PointLight(vec3 color);
    void apply(Shader& s, const string& target) override;
};

//聚光 本身具有Transform
class SpotLight : public Light {
public:
    Transform transform;
    vec3 decay;
    vec3 direction; //照射方向
    float angleIn, angleOut; //角度值
    
    SpotLight(vec3 color, float agin, float agout, vec3 direction = vec3(0, -1, 0));
    void apply(Shader& s, const string& target) override;
};
