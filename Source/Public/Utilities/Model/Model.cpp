#include "Utilities/Model/Model.h"
#include "Utilities/Logger.h"

Model::Model(std::string path)
{
	LoadModel(path);
}

void Model::Draw(Shader& shader)
{
	for (auto& Mesh : Meshes) {
		Mesh.Draw(shader);
	}
}

void Model::LoadModel(std::string path)
{
	Assimp::Importer import;
	const aiScene*  scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		Logger::LogDebug("Model::LoadModel()", "SCENE LOAD FAIL");
		return;
	}
	Path = std::filesystem::path(path).parent_path().string();
	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* Node, const aiScene* scene)
{
	for (unsigned int i = 0; i < Node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[Node->mMeshes[i]];
		Meshes.push_back(ProcessMesh(mesh, scene));
	}
	for (unsigned int i = 0; i < Node->mNumChildren; i++) {
		ProcessNode(Node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	VertexList vertices;
	IndexList indices;
	TextureList textures;
	
	for (unsigned int i= 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;

		vertex.Normal.x = mesh->mNormals[i].x;
		vertex.Normal.y = mesh->mNormals[i].y;
		vertex.Normal.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0]) {
			vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		TextureList diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		TextureList specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string name)
{
	TextureList textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		auto texture = LoadCachedTexture(GL_TEXTURE_2D, Path + "/" + str.C_Str());
		textures.push_back(texture);
	}
	return textures;
}
