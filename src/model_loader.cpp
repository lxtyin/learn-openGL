#include <fstream>
#include <iostream>
#include "model_loader.h"
#include "exglm.hpp"
using namespace std;

const float INTERVAL = 0.2; //水平间距
const float HISCALE = 0.01; //高度比例

Dem_Mesh dem_loader(const char* path) { //获得mx_height

    vector<Vertex> vers;
    vector<uint> ids;
    std::ifstream fin(path);

    float useless;
    fin >> useless >> useless;
    int W, H;
    fin >> W >> H;
    fin >> useless >> useless >> useless;

    float ht[W][H];
    for(int i = 0;i < W;i++) {
        for(int j = 0;j < H;j++) {
            fin >> ht[i][j];
            vers.push_back(Vertex(
                (i - W / 2) * INTERVAL, ht[i][j] * HISCALE, (j - H / 2) * INTERVAL, //顶点坐标
                0, 1, 0,                                                            //法线暂定
                j * 1.0 / H, i * 1.0 / W));                                         //纹理坐标
        }
    }

    auto id = [&](int i, int j) {
        return i * H + j;
    };
    auto lineto = [&](int idx, int idy) {
        return glm::vec3(
            vers[idy].pos[0] - vers[idx].pos[0],
            vers[idy].pos[1] - vers[idx].pos[1],
            vers[idy].pos[2] - vers[idx].pos[2]
        );
    };

    for(int i = 0;i < W;i++) {
        for(int j = 0;j < H;j++) {
            //按各面均值计算法线
            glm::vec3 v[4];
            if(i != 0) v[0] = lineto(id(i, j), id(i - 1, j));     //<
            if(j != 0) v[1] = lineto(id(i, j), id(i, j - 1));     //v
            if(i + 1 != W) v[2] = lineto(id(i, j), id(i + 1, j)); //>
            if(j + 1 != H) v[3] = lineto(id(i, j), id(i, j + 1)); //^
            glm::vec3 normal = glm::normalize(
                glm::cross(v[0], v[3]) +
                glm::cross(v[1], v[0]) +
                glm::cross(v[2], v[1]) +
                glm::cross(v[3], v[2]));
            vers[id(i, j)].nor[0] = normal[0];
            vers[id(i, j)].nor[1] = normal[1];
            vers[id(i, j)].nor[2] = normal[2];
            // cout << normal[0] << ' ' << normal[1] << ' ' << normal[2] << '\n';
        }
    }
    for(int i = 1;i < W;i++) {
        for(int j = 1;j < H;j++) {
            ids.push_back(id(i, j));
            ids.push_back(id(i, j - 1));
            ids.push_back(id(i - 1, j));
            ids.push_back(id(i, j - 1));
            ids.push_back(id(i - 1, j - 1));
            ids.push_back(id(i - 1, j));
        }
    }
    fin.close();
    return Dem_Mesh(vers, ids);
}