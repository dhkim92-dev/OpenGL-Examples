#ifndef __GL_EXAMPLE_MESH_H__
#define __GL_EXAMPLE_MESH_H__

#include <vector>
#include <string>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "shader.h"

using namespace std;

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 tex;
	glm::vec3 tan;
	glm::vec2 btan;
};

struct Texture{
 	uint32_t id;
	string type;
};

class Mesh
{
	protected : 
	vector<Vertex> vertices;
	vector<uint32_t> indices;
	vector<Texture> textures;
	unsigned int vao, vbo, ibo;

	void build();
public:
	Mesh();
	Mesh(vector<Vertex> &vertices, vector<uint32_t> &indices, vector<Texture> textures);
	void render();
	void setVertices(vector<Vertex> &buffer);
	void setIndices(vector<uint32_t> &buffer);
	unsigned int getVAO();
	unsigned int getVBO();
	unsigned int getIBO();
};

#endif