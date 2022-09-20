#include "Light.h"
#include "tool.h"
#include <iostream>

Light::Light(vec3 color) :
    specular(color),
    ambient(color* DEFAULT_AMBIENT),
    diffuse(color* DEFAULT_DIFFUSE) {
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

ParallelLight::ParallelLight(vec3 color, vec3 direction) :
    Light(color),
    direction(direction) {}

PointLight::PointLight(vec3 color) :
    Light(color),
    decay(DEFAULT_DECAY),
    transform(Transform()) {}

SpotLight::SpotLight(vec3 color, float agin, float agout, vec3 direction) :
    Light(color),
    decay(DEFAULT_DECAY),
    transform(Transform()),
    direction(direction),
    angleIn(agin),
    angleOut(agout) {}

void Light::setAmbientStrength(float r) {
    assert(r > 0);
    ambient = specular * r;
}

void Light::setDiffuseStrength(float r) {
    assert(r > 0);
    diffuse = specular * r;
}

void Light::apply(Shader& s, const string& target) {
    s.setVec3(target + ".ambient", ambient);
    s.setVec3(target + ".diffuse", diffuse);
    s.setVec3(target + ".specular", specular);
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
