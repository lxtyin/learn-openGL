#pragma once

#include "exglm.hpp"
#include "Texture.h"
#include <vector>
using std::vector;
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

//网格即一般的模型表现方式，除此之外还有点云，体素等等
//仅关注点的绘制，贴图和shader的设置不关心
class Mesh {
protected:
    vector<Vertex> vertices;
    vector<uint> indices;
    unsigned int VAO, VBO, EBO;
public:
    // Mesh(float* datasource);
    Mesh(vector<Vertex> vers); //不使用EBO
    Mesh(vector<Vertex> vers, vector<uint> ids); //使用EBO
    void Draw();
};

class Dem_Mesh : public Mesh {
public:
    float mx_height;
    Dem_Mesh(vector<Vertex> vers); //不使用EBO
    Dem_Mesh(vector<Vertex> vers, vector<uint> ids); //使用EBO
};