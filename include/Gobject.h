#pragma once

#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"
#include <vector>
using namespace std;

//简易物品类，默认采用三角方式渲染
class Gobject {
    unsigned int vao;
    vector<pair<Texture, int>> textures;
    Shader shader;
public:
    Transform transform; //相对父物体的transform

    Gobject(unsigned int vao, const Shader& sd);
    void addTexture(const Texture& t, int tar);


};