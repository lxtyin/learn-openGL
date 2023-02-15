#include "Light.h"
#include <iostream>

Light::Light(vec3 c) : color(c) {}

DirectionalLight::DirectionalLight(vec3 c, vec3 direction) :
    Light(c),
    direction(direction) {}

PointLight::PointLight(vec3 c) :
    Light(c),
    decay(DEFAULT_DECAY),
    transform(Transform()) {}

SpotLight::SpotLight(vec3 c, float agin, float agout, vec3 direction) :
    Light(c),
    decay(DEFAULT_DECAY),
    transform(Transform()),
    direction(direction),
    angleIn(agin),
    angleOut(agout) {}

SurroundLight::SurroundLight(vec3 c) :
        Light(c){}

void Light::apply(Shader& s, const string& name) {
    s.setVec3(name + ".color", color);
}

void DirectionalLight::apply(Shader& s, const string& name) {
    Light::apply(s, name);
    s.setVec3(name + ".direction", direction);
    s.setInt(name + ".type", TYPE_DIRECTION);
}

void PointLight::apply(Shader& s, const string& name) {
    Light::apply(s, name);
    s.setVec3(name + ".position", transform.position);
    s.setVec3(name + ".decay", decay);
    s.setInt(name + ".type", TYPE_POINT);
}

void SpotLight::apply(Shader& s, const string& name) {
    Light::apply(s, name);
    s.setVec3(name + ".position", transform.position);
    s.setVec3(name + ".decay", decay);
    s.setFloat(name + ".inCut", glm::cos(glm::radians(angleIn)));
    s.setFloat(name + ".outCut", glm::cos(glm::radians(angleOut)));
    s.setInt(name + ".type", TYPE_SPOT);

    vec3 dir = transform.matrix() * vec4(direction, 0.0); //计算direction在世界坐标下的方向，注意不位移
    s.setVec3(name + ".direction", dir);
}

void SurroundLight::apply(Shader &s, const string &name) {
    Light::apply(s, name);
    s.setInt(name + ".type", TYPE_SURROUND);
}