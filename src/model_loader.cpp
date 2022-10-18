
#include "model_loader.h"
#include <fstream>
#include <iostream>

using namespace std;
Dem_Mesh dem_loader(const char* path) { //获得mx_height
    vector<Vertex> vers;
    vector<uint> ids;
    std::ifstream fin(path);

    float useless;
    fin >> useless >> useless;
    int W, H;
    fin >> W >> H;
    fin >> useless >> useless >> useless;
    
    for(int i = 0;i < W;i++) {
        for(int j = 0;j < H;j++) {
            float ht; fin >> ht;
            vers.push_back(Vertex((i - W / 2) * 20.0 / W, ht / 120, (j - H / 2) * 20.0 / H, 0, 1, 0, i * 1.0 / W, j * 1.0 / H)); //法线暂定
        }
    }
    auto id = [&](int i, int j) {
        return i * H + j;
    };
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