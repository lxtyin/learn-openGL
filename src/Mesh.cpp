#include "Mesh.h"
#include "tool.h"
#include <iostream>

Mesh::Mesh(const vector<Vertex> &vers) :
    vertices(vers), EBO(0) {
    //建立对象
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //利用内存的连续性读取vector，用struct的offsetof获取偏移（因为结构体保留偏移）
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
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

Mesh::Mesh(const vector<Vertex> &vers, const vector<uint> &ids) :
    vertices(vers), indices(ids) {
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

void Mesh::addTexture(const Texture &tex, int type){
    if(type == TYPE_DIFFUSE) diffuse_tex.push_back(tex);
    else specular_tex.push_back(tex);
}

Dem_Mesh::Dem_Mesh(const vector<Vertex> &vers) :
    Mesh(vers) {
    mx_height = -1e9;
    for(auto& v : vertices) {
        if(v.pos[1] > mx_height) mx_height = v.pos[1];
    }
}

Dem_Mesh::Dem_Mesh(const vector<Vertex> &vers, const vector<uint> &ids) :
    Mesh(vers, ids) {
    mx_height = -1e9;
    for(auto& v : vertices) {
        if(v.pos[1] > mx_height) mx_height = v.pos[1];
    }
}

void Mesh::draw(Shader &shader) const {
    int id = 0;
    for(int i = 0;i < diffuse_tex.size();i++){
        diffuse_tex[i].activeTarget(id);
        shader.setInt(str_format("material.diffuseMap_%d", i + 1), id++);
    }
    for(int i = 0;i < specular_tex.size();i++){
        specular_tex[i].activeTarget(id);
        shader.setInt(str_format("material.specularMap_%d", i + 1), id++);
    }

    glBindVertexArray(VAO);
    if(EBO) glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    else glDrawArrays(GL_TRIANGLES, 0, vertices.size());
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
