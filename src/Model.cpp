//
// Created by lx_tyin on 2022/11/4.
//

#include <iostream>
#include "Model.h"
#include "stb_image.h"

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    vector<Vertex> vers;
    vector<uint> ids;
    // 读取所有顶点数据
    for(int i = 0; i < mesh->mNumVertices; i++){
        float tex[2] = {0, 0};
        if(mesh->mTextureCoords[0]) { //如果有纹理坐标就取第一个
            tex[0] = mesh->mTextureCoords[0][i].x;
            tex[1] = mesh->mTextureCoords[0][i].y;
        }
        vers.emplace_back(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z,
                          mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z,
                          tex[0], tex[1]);
    }

    // 网格由三角面片组成，assimp用到EBO来表示每一个面
    for(int i = 0; i < mesh->mNumFaces; i++){
        aiFace &face = mesh->mFaces[i];
        for(int j = 0; j < face.mNumIndices; j++)
            ids.push_back(face.mIndices[j]);
    }

    Mesh result(vers, ids);

    // 读取材质
    if(mesh->mMaterialIndex >= 0){
        aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];
        for(int i = 0;i < mat->GetTextureCount(aiTextureType_SPECULAR);i++){
            aiString str;
            mat->GetTexture(aiTextureType_SPECULAR, i, &str);
            string path = directory + string(str.C_Str());
            result.addTexture(Texture(path.c_str()), TYPE_SPECULAR);
        }
        for(int i = 0;i < mat->GetTextureCount(aiTextureType_DIFFUSE);i++){
            aiString str;
            mat->GetTexture(aiTextureType_DIFFUSE, i, &str);
            string path = directory + string(str.C_Str());
            result.addTexture(Texture(path.c_str()), TYPE_DIFFUSE);
        }
    }
    return result;
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    for(int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    for(int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Model::Model(const string &path) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
    // 第二个参数为预处理指令，此处指定：全部转换为三角形 | 翻转纹理y轴 | 自动生成法线

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << '\n';
        return;
    }
    directory = path;
    while(!directory.empty() && directory.back() != '/' && directory.back() != '\\') directory.pop_back();

    // 递归获得网格（仅数据
    processNode(scene->mRootNode, scene);
    std::cout << "The number of meshes in " << directory << " : " << meshes.size() << '\n';
}

// 绘制所有
void Model::draw(Shader &shader) {
    for(Mesh &m: meshes){
        m.draw(shader);
    }
}