#ifndef __GLEX_MODEL_H__
#define __GLEX_MODEL_H__
#include <iostream>
#include <vector>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#elif __linux__
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif
#include "mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

using namespace std;

uint32_t glexLoadTextureFromFile(const char *file_name, const string dir_path, bool gamma=false);

class Model
{
public:
	Model(const string file_path, bool gamma=false);
	string dir_path;
	vector<Mesh> meshes;
	vector<Texture> texture_loaded;
	bool gamma;

	void render(Shader *shader);
private:
	aiScene *scene;

	void loads(const string file_path);
	void buildNode(aiNode *node, const aiScene *scene);
	Mesh buildMesh(aiMesh *mesh, const aiScene *scene);
	vector<Vertex> loadVertices(aiMesh *mesh);
	vector<uint32_t> loadIndices(aiMesh *mesh);
	vector<Texture> loadTextures(aiMaterial *material, aiTextureType type, string type_name);
};

#endif
