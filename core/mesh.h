#ifndef __GL_EXAMPLE_MESH_H__
#define __GL_EXAMPLE_MESH_H__

#include <vector>
#include <string>
#include <glm/glm.hpp>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#elif __linux__
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#include <GLFW/glfw3.h>
#include "shader.h"

using namespace std;


#define MAX_BONE_INFLUENCE 4

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec3 tan;
	glm::vec2 bitangents;
	int bone_IDs[MAX_BONE_INFLUENCE];
	float bone_weights[MAX_BONE_INFLUENCE];
};

struct Texture{
 	uint32_t id;
	string type;
	string path;
};

class Mesh
{
	protected:
	uint32_t vao, vbo, ibo;
	public:
	vector<Vertex> vertices;
	vector<uint32_t> indices;
	vector<Texture> textures;
	Mesh(vector<Vertex> vertices, vector<uint32_t> indices, vector<Texture> textures);
	void build();
	void render(Shader *shader);
	uint32_t getVAO();
	uint32_t getVBO();
	uint32_t getIBO();
};

#endif