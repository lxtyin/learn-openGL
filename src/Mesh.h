#pragma once

#include "tool/exglm.hpp"
#include "Texture.h"
#include "Shader.h"
#include "material/Material.h"
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

class Mesh {
protected:
    vector<Vertex> vertices;
    vector<uint> indices;
    unsigned int VAO, VBO, EBO;

	bool is_load = false;
public:

	void load();	/** < load to gpu */
	void unload();	/** < unload from gpu */

    explicit Mesh(const vector<Vertex> &vers); //默认EBO
    Mesh(const vector<Vertex> &vers, const vector<uint> &ids); //指定EBO

    void draw();
};

extern const vector<Vertex> BOX_MESH_SOURCE;
