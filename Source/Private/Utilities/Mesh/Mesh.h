#pragma once
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Utilities/Shader/Shader.h"
struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 TexCoords;
};

struct Texture
{
	unsigned int Texture;
};

class Mesh
{
public:
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;
	std::vector<Texture> Textures;
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
private:
	GLuint VAO, VBO, EBO;
	void SetupMesh();
};

