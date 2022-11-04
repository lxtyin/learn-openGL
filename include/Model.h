//
// Created by lx_tyin on 2022/11/4.
//

#ifndef OPENGL_MODEL_H
#define OPENGL_MODEL_H

#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
public:
    vector<Mesh> meshes;
    Model(const char *path);
    void draw();

};


#endif //OPENGL_MODEL_H
