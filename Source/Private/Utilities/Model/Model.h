#pragma once
#include <assimp/Importer.hpp>
#include <assimp/Scene.h>
#include <assimp/PostProcess.h>
#include "Utilities/Shader/Shader.h"
#include "Utilities/Model/Mesh.h"

class Model
{
public:
	Model() = default;
	Model(std::string path);
	void Draw(Shader& shader);
private:
	std::vector<Mesh> Meshes;
	std::string Path;

	void LoadModel(std::string path);
	void ProcessNode(aiNode* Node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* Mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string name);
};

