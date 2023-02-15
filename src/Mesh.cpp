#include "Mesh.h"
#include "tool/tool.h"
#include <iostream>

void Mesh::load() {
	is_load = true;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), &indices[0], GL_STATIC_DRAW);

    //位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(0);
    //法线属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, nor));
    glEnableVertexAttribArray(1);
    //纹理坐标属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);//unbind VAO
}

void Mesh::unload() {
	is_load = false;
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

Mesh::Mesh(const vector<Vertex> &vers) :
    vertices(vers), indices(vers.size(), 0) {
    for(int i = 0;i < vers.size();i++) indices[i] = i;
}

Mesh::Mesh(const vector<Vertex> &vers, const vector<uint> &ids) :
    vertices(vers), indices(ids) {}

void Mesh::draw() {
	if(!is_load) load();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

const vector<Vertex> BOX_MESH_SOURCE {
    //pos                //nor                //tex
    {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0, 0},
    {0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,    1, 0},
    {0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,    1, 1},
    {0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,    1, 1},
    {-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0, 1},
    {-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0, 0},

    {-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    0, 0},
    {0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,     1, 0},
    {0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,     1, 1},
    {0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,     1, 1},
    {-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    0, 1},
    {-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    0, 0},

    {-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   0, 0},
    {-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   1, 0},
    {-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   1, 1},
    {-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   1, 1},
    {-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   0, 1},
    {-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   0, 0},

    {0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,    0, 0},
    {0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,    1, 0},
    {0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,    1, 1},
    {0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,    1, 1},
    {0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,    0, 1},
    {0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,    0, 0},

    {-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   0, 0},
    {0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,    1, 0},
    {0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,    1, 1},
    {0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,    1, 1},
    {-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   0, 1},
    {-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   0, 0},

    {-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   0, 0},
    {0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,    1, 0},
    {0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,    1, 1},
    {0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,    1, 1},
    {-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   0, 1},
    {-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   0, 0}
};
