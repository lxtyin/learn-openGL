#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../Mesh.h"
#include "../Texture.h"
#include "../Instance.h"
#include "../Material.h"

namespace Loader{
	/**
	 * load a dem mesh.
	 * \param path dem file path.
	 * \param max_height out max height.
	 * \return a dem mesh.
	 */
	Mesh* load_dem(const string &path, float &low_height, float &max_height);

	Texture*    processImage(const string &name, const aiScene *scene);

	Material* processMaterial(aiMaterial *mat, const aiScene *scene);
	Instance* processNode(aiNode *node, const aiScene *scene, Instance *t_node);
	Instance* processMesh(aiMesh *mesh, const aiScene *scene, Instance *t_node);
	Instance* load_model(const string &path);

}




