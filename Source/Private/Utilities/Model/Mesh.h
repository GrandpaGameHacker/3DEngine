#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Utilities/Texture/Texture.h"
#include "Utilities/Shader/Shader.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

typedef std::vector<Vertex> VertexList;
typedef std::vector<unsigned int> IndexList;
typedef std::vector<Texture> TextureList;

class Mesh
{

public:
	Mesh();
	Mesh(VertexList vertices, IndexList indices, TextureList textures);
	void Draw(Shader& shader);
private:
	void SetupMesh();

public:
	VertexList Vertices;
	IndexList Indices;
	TextureList Textures;
private:
	GLuint VAO, VBO, EBO;
};

