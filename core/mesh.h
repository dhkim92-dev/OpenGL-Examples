#ifndef __GL_EXAMPLE_MESH_H__
#define __GL_EXAMPLE_MESH_H__

#include <vector>
#include <string>
#include <glm/glm.hpp>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/gl.h>
#endif
#include <GLFW/glfw3.h>
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

public:
	Mesh();
	Mesh(vector<Vertex> &vertices, vector<uint32_t> &indices, vector<Texture> textures);
	void build();
	void render();
	void setVertices(vector<Vertex> &buffer);
	void setIndices(vector<uint32_t> &buffer);
	unsigned int getVAO();
	unsigned int getVBO();
	unsigned int getIBO();
};

#endif