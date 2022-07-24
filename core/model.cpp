#ifndef __GLEX_MODEL_CPP__
#define __GLEX_MODEL_CPP__

#include "model.h"


Model::Model(const string file_path, bool gamma)
: gamma(gamma)
{
	loads(file_path);
}

void Model::render(Shader *shader)
{
	for(int i = 0 ; i < meshes.size() ; i++)
	{
		meshes[i].render(shader);
	}
}

void Model::loads(const string file_path)
{
	Assimp::Importer importer;
	cout << "loading model file : " << file_path << endl;
	const aiScene *scene = importer.ReadFile(file_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	// const aiScene *scene = importer.ReadFile(file_path.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);// | aiProcess_CalcTangentSpace);
	cout << "loading complete : " << file_path << endl;

	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "read file failed\n";
		cout << "Error::Assimp read file fail" << importer.GetErrorString() << endl;
		return ;
	}

	dir_path = file_path.substr(0, file_path.find_last_of('/'));
	buildNode(scene->mRootNode, scene);
}

void Model::buildNode(aiNode *node, const aiScene *scene)
{
	for(uint32_t i = 0 ; i < node->mNumMeshes ; i++){
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(buildMesh(mesh, scene));
	}

	for(uint32_t i = 0 ; i < node->mNumChildren ; i++){
		buildNode(node->mChildren[i], scene);
	}
}

Mesh Model::buildMesh(aiMesh *mesh, const aiScene *scene)
{
	vector<Vertex> _vertices = loadVertices(mesh);
	cout << "mesh vertices size : " << _vertices.size();
	vector<uint32_t> _indices = loadIndices(mesh);
	cout << "mesh indices size : " << _indices.size();
	vector<Texture> _textures;

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	vector<Texture> diffuses = loadTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	vector<Texture> speculars = loadTextures(material, aiTextureType_SPECULAR, "texture_specular");
	vector<Texture> normal_maps = loadTextures(material, aiTextureType_HEIGHT, "texture_normal");
	vector<Texture> ambients = loadTextures(material, aiTextureType_AMBIENT, "texture_height");
	_textures.insert(_textures.end(), diffuses.begin(), diffuses.end());
	_textures.insert(_textures.end(), speculars.begin(), speculars.end());
	_textures.insert(_textures.end(), normal_maps.begin(), normal_maps.end());
	_textures.insert(_textures.end(), ambients.begin(), ambients.end());
	cout << "mesh texture size : " << _textures.size();
	return Mesh(_vertices, _indices, _textures);
}

vector<Vertex> Model::loadVertices(aiMesh* mesh)
{
	vector<Vertex> _vertices;
	for(uint32_t i = 0 ; i < mesh->mNumVertices ; i++)
	{
		Vertex vtx;
		glm::vec3 placeholder;

		placeholder.x = mesh->mVertices[i].x;
		placeholder.y = mesh->mVertices[i].y;
		placeholder.z = mesh->mVertices[i].z;
		vtx.pos = placeholder;

		if(mesh->HasNormals())
		{
			placeholder.x = mesh->mNormals[i].x;
			placeholder.y = mesh->mNormals[i].y;
			placeholder.z = mesh->mNormals[i].z;
			vtx.normal = placeholder;
		}

		if(mesh->mTextureCoords[0])
		{
			glm::vec2 uv;
			uv.x = mesh->mTextureCoords[0][i].x;
			uv.y = mesh->mTextureCoords[0][i].y;
			vtx.uv = uv;

			placeholder.x = mesh->mTangents[i].x;
			placeholder.y = mesh->mTangents[i].y;
			placeholder.z = mesh->mTangents[i].z;
			vtx.tan = placeholder;

			placeholder.x = mesh->mBitangents[i].x;
			placeholder.y = mesh->mBitangents[i].y;
			placeholder.z = mesh->mBitangents[i].z;
			vtx.bitangents = placeholder;
		}else{
			vtx.uv = glm::vec2(0.0f, 0.0f);
		}

		_vertices.push_back(vtx);
	}

	return _vertices;
}

vector<uint32_t> Model::loadIndices(aiMesh *mesh)
{
	vector<uint32_t> _indices;
	_indices.reserve(mesh->mNumFaces * 3 * sizeof(uint32_t));
	for(uint32_t i = 0 ; i < mesh->mNumFaces ; i++)
	{
		aiFace face = mesh->mFaces[i];
		vector<uint32_t> part_indices;
		part_indices.resize(face.mNumIndices);
		for(uint32_t j = 0 ; j < face.mNumIndices; j++)
		{
			part_indices[j] = face.mIndices[j];
		}
		_indices.insert(_indices.end(), part_indices.begin(), part_indices.end());
	}
	return _indices;
}

vector<Texture> Model::loadTextures(aiMaterial *material, aiTextureType type, string type_name)
{
	vector<Texture> _textures;

	for(uint32_t i = 0 ; i < material->GetTextureCount(type) ; i++){
		aiString str;
		material->GetTexture(type, i, &str);
		bool skip = false;

		for(uint32_t j = 0 ; j < texture_loaded.size() ; j++){
			if(std::strcmp(texture_loaded[j].path.c_str(), str.C_Str())==0){
				_textures.push_back(texture_loaded[j]);
				skip = true;
				break;
			}
		}

		if(!skip){
			Texture texture;
			texture.id = glexLoadTextureFromFile(str.C_Str(), dir_path, gamma);
			texture.type = type;
			texture.path = str.C_Str();
			_textures.push_back(texture);
			texture_loaded.push_back(texture);
		}
	}

	return _textures;
}

uint32_t glexLoadTextureFromFile(const char *file_name, const string dir_path, bool gamma)
{
	const string file_path = dir_path + "/" + string(file_name);
	uint32_t tex_id;
	glGenTextures(1, &tex_id);
	int width, height, components;
	uint8_t *ptr = stbi_load(file_path.c_str(), &width, &height, &components, 0);
	
	if(ptr)
	{
		GLenum format;
		if(components == 1){
			format = GL_RED;
		}else if(components == 3){
			format = GL_RGB;
		}else if(components == 4){
			format = GL_RGBA;	
		}
		glBindTexture(GL_TEXTURE_2D, tex_id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, ptr);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}else{
		cout << "Fail to load textures path : " << file_path << endl;
	}
	stbi_image_free(ptr);
	return tex_id;
}
#endif