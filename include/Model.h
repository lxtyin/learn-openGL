//
// Created by lx_tyin on 2022/11/4.
//

#ifndef OPENGL_MODEL_H
#define OPENGL_MODEL_H

#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using std::string;

class Model {
    vector<Mesh> meshes;
    string directory; // 所在目录
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
public:
    Model(const string &path);
    void draw(Shader &shader);

};


#endif //OPENGL_MODEL_H
