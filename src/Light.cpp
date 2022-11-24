#include "Light.h"
#include "tool.h"
#include <iostream>

Light::Light(vec3 c) : color(c) {
    all_lights.insert(this);
}

Light::~Light() {
    all_lights.erase(this);
}

set<Light*> Light::all_lights;
void Light::applyAllLightTo(Shader& s) {
    int n = 0;
    for(Light* l : all_lights) {
        l->apply(s, str_format("light[%d]", n++));
    }
    s.setInt("light_N", n);
}

ParallelLight::ParallelLight(vec3 c, vec3 direction) :
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

void Light::apply(Shader& s, const string& target) {
    s.setVec3(target + ".color", color);
}

void ParallelLight::apply(Shader& s, const string& target) {
    Light::apply(s, target);
    s.setVec3(target + ".direction", direction);
    s.setInt(target + ".type", TYPE_PARALLEL);
}

void PointLight::apply(Shader& s, const string& target) {
    Light::apply(s, target);
    s.setVec3(target + ".position", transform.position());
    s.setVec3(target + ".decay", decay);
    s.setInt(target + ".type", TYPE_POINT);
}

void SpotLight::apply(Shader& s, const string& target) {
    Light::apply(s, target);
    s.setVec3(target + ".position", transform.position());
    s.setVec3(target + ".decay", decay);
    s.setFloat(target + ".inCut", glm::cos(glm::radians(angleIn)));
    s.setFloat(target + ".outCut", glm::cos(glm::radians(angleOut)));
    s.setInt(target + ".type", TYPE_SPOT);

    vec3 dir = transform.transmat * vec4(direction, 0.0); //计算direction在世界坐标下的方向，注意不位移
    s.setVec3(target + ".direction", dir);
}

void SurroundLight::apply(Shader &s, const string &target) {
    Light::apply(s, target);
    s.setInt(target + ".type", TYPE_SURROUND);
}