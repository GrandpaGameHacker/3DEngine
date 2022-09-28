#include "Utilities/Model/Mesh.h"
#include "Utilities/Logger.h"
Mesh::Mesh()
{
	Logger::LogDebug("Mesh::Mesh(NOT_IMPLEMENTED)", "");
	exit(0);
}

Mesh::Mesh(VertexList vertices, IndexList indices, TextureList textures)
{
	Vertices = vertices;
	Indices = indices;
	Textures = textures;
	SetupMesh();
}

void Mesh::Draw(Shader& shader)
{
	unsigned int diffuseNumber = 1;
	unsigned int specularNumber = 1;
	for (unsigned int i = 0; i < Textures.size(); i++) {
		std::string number;
		std::string name;
		Textures[i].GetName(name);
		if (name == "diffuse") {
			number = std::to_string(diffuseNumber++);
		}
		else if (name == "specular") {
				number = std::to_string(specularNumber++);
		}
		else {
			Logger::LogDebug("Mesh::Draw()", "Texture not named specular/diffuse");
		}
		shader.Set("material." + name + number, i);
		Textures[i].Bind(i);
	}
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), Vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), Indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}
