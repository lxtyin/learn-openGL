#pragma once

#include "exglm.hpp"
#include "Texture.h"
#include "Shader.h"
#include <vector>
#include <string>
using std::vector;
using std::pair;
using std::string;
using uint = unsigned int;

//一般的顶点包括的信息都是：位置，法向量，纹理坐标
struct Vertex {
    float pos[3];
    float nor[3];
    float tex[2];
    Vertex(float p0, float p1, float p2, float n0, float n1, float n2, float t0, float t1) {
        pos[0] = p0, pos[1] = p1, pos[2] = p2;
        nor[0] = n0, nor[1] = n1, nor[2] = n2;
        tex[0] = t0, tex[1] = t1;
    }
};

const int TYPE_DIFFUSE = 1;
const int TYPE_SPECULAR = 2;

class Mesh {
protected:
    vector<Vertex> vertices;
    vector<uint> indices;
    vector<Texture> diffuse_tex, specular_tex;
    unsigned int VAO, VBO, EBO;
public:
    // Mesh(float* datasource);
    Mesh(const vector<Vertex> &vers); //不使用EBO
    Mesh(const vector<Vertex> &vers, const vector<uint> &ids); //使用EBO
    void addTexture(const Texture &tex, int type);
    void draw(Shader &shader) const;
};

class Dem_Mesh : public Mesh {
public:
    float mx_height;
    Dem_Mesh(const vector<Vertex> &vers); //不使用EBO
    Dem_Mesh(const vector<Vertex> &vers, const vector<uint> &ids); //使用EBO
};

extern const vector<Vertex> BOX_MESH_SOURCE;
