#include <fstream>
#include <iostream>
#include "loader.h"
#include "exglm.hpp"
#include "../material/DisneyMaterial.h"
#include "../material/PhongMaterial.h"
using namespace std;

const float INTERVAL = 0.2; //水平间距
const float HISCALE = 0.02; //高度比例

namespace Loader{
	Mesh* load_dem(const string &path, float &low_height, float &max_height) {
		vector<Vertex> vers;
		vector<uint> ids;
		std::ifstream fin(path);

		float useless;
		fin >> useless >> useless;
		int W, H;
		fin >> W >> H;
		fin >> useless >> useless >> useless;

		low_height = 1e9;
		max_height = -1e9;
		float ht[W][H];
		for(int i = 0;i < W;i++) {
			for(int j = 0;j < H;j++) {
				fin >> ht[i][j];
				vers.push_back(Vertex(
					(i - W / 2) * INTERVAL, ht[i][j] * HISCALE, (j - H / 2) * INTERVAL, //顶点坐标
					0, 1, 0,                                                            //法线暂定
					j * 1.0 / H, i * 1.0 / W));                                         //纹理坐标
				max_height = max(max_height, ht[i][j] * HISCALE);
				low_height = min(low_height, ht[i][j] * HISCALE);
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
		return new Mesh(vers, ids);
	}

	//loading directory
	string directory;

	Texture* processImage(const string &name, const aiScene *scene){
		int width, height, channel;
		if(name[0] == '*'){
			// embedded texture
			auto tex = scene->GetEmbeddedTexture(name.c_str());
			int siz = tex->mHeight ? tex->mWidth * tex->mHeight : tex->mWidth; //maybe compressed
			unsigned char *buff = stbi_load_from_memory(reinterpret_cast<unsigned char*>(tex->pcData),
				siz,
				&width,
				&height,
				&channel, 0);

			vector<uint8_t> data(width * height * channel);
			memcpy(data.data(), buff, data.size());
			stbi_image_free(buff);
			return new Texture(width, height, channel, data);
		} else {
			// local image file.
			string path = directory + name;
			return new Texture(path);
		}
	}

	Material* processMaterial(aiMaterial *mat, const aiScene *scene){
        // 暂且固定disney material
        DisneyMaterial *result = new DisneyMaterial;

		// 获取各项属性
		aiColor3D color;
		if(AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_DIFFUSE, color)){
			result->base_color = vec3(color.r, color.g, color.b);
		}

		// 获取纹理
		for(int i = 0;i < mat->GetTextureCount(aiTextureType_DIFFUSE);i++){
			aiString str;
			mat->GetTexture(aiTextureType_DIFFUSE, i, &str);
			result->diffuse_map = processImage(str.C_Str(), scene);
		}

		return result;
	}

	Instance* processMesh(aiMesh *mesh, const aiScene *scene) {
		Instance *result = new Instance;

		vector<Vertex> vers;
		vector<unsigned int> ids;

		// 读取所有顶点数据
		for(int i = 0; i < mesh->mNumVertices; i++){
			float tex[2] = {0, 0};
			if(mesh->mTextureCoords[0]) { //如果有纹理坐标就取第一个 ?
				tex[0] = mesh->mTextureCoords[0][i].x;
				tex[1] = mesh->mTextureCoords[0][i].y;
			}
			vers.emplace_back(
				mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z,
				mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z,
				tex[0], tex[1]
			);

		}
		for(int i = 0; i < mesh->mNumFaces; i++){
			aiFace &face = mesh->mFaces[i];
			for(int j = 0; j < face.mNumIndices; j++){
				ids.push_back(face.mIndices[j]);
			}
		}
		result->mesh = new Mesh(vers, ids);

		// 获取材质
		if(mesh->mMaterialIndex >= 0){
			aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];
			result->material = processMaterial(mat, scene);
		}
		return result;
	}

	Instance* processNode(aiNode *node, const aiScene *scene) {
		Instance *cur = new Instance;

		aiVector3D position, rotation, scale;
		node->mTransformation.Decompose(scale, rotation, position);
		cur->transform.position = vec3(position.x, position.y, position.z);
		cur->transform.rotation = vec3(rotation.x, rotation.y, rotation.z);
		cur->transform.scale    = vec3(scale.x,    scale.y,    scale.z    );

		for(int i = 0; i < node->mNumMeshes; i++) {
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
			cur->add_child(processMesh(mesh, scene));
		}
		for(int i = 0; i < node->mNumChildren; i++) {
			cur->add_child(processNode(node->mChildren[i], scene));
		}
		return cur;
	}

	Instance* load_model(const string &path){
		Assimp::Importer import;
		const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
		// 第二个参数为预处理指令，此处指定：全部转换为三角形 | 翻转纹理y轴 | 自动生成法线

		if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
			std::cerr<<"ERROR::ASSIMP::"<<import.GetErrorString()<<'\n';
			return nullptr;
		}
		directory = path;
		while(!directory.empty() && directory.back() != '/' && directory.back() != '\\') directory.pop_back();

		Instance *result = processNode(scene->mRootNode, scene);
		// 有时会莫名整个旋转
		result->transform.scale = vec3(1);
		result->transform.rotation = vec3(0);
		result->transform.position = vec3(0);
		return result;
	}
}

