#ifndef __GL_EXAMPLE_MESH_CPP__
#define __GL_EXAMPLE_MESH_CPP__

#include <iostream>
#include <cstdio>
#include "mesh.h"

using namespace std;

Mesh::Mesh(
	vector<Vertex> vertices, 
	vector<uint32_t> indices,
	vector<Texture> textures) 
	: vertices(vertices), indices(indices), textures(textures)
{
	build();
	printf("mesh generated : sz_vertices(%d) sz_indices(%d), sz_textures(%d\n)", vertices.size(), indices.size(), textures.size());
}

void Mesh::build()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex),
				vertices.data(), GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(uint32_t), 
				indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));
	
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tan));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangents));
	glBindVertexArray(0);
}

void Mesh::render(Shader *shader)
{
	uint32_t nr_diffuse = 1;
	uint32_t nr_specular = 1;
	uint32_t nr_normal = 1;
	uint32_t nr_height = 1;

	for(unsigned int i = 0 ; i < textures.size() ; i++){
		glActiveTexture(GL_TEXTURE0 + i);
		string number;
		string name = textures[i].type;

		if(name == "texture_diffuse")
			number = std::to_string(nr_diffuse++);
		else if(name == "texture_specular")
			number = std::to_string(nr_specular++);
		else if(name == "texture_normal")
			number = std::to_string(nr_normal++);
		else if(name == "texture_height")
			number = std::to_string(nr_height++);

		glUniform1i(glGetUniformLocation(shader->getProgramID(), (name+number).c_str()),i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

uint32_t Mesh::getVAO()
{
	return vao;
}

uint32_t Mesh::getVBO()
{
	return vbo;
}

uint32_t Mesh::getIBO()
{
	return ibo;
}

#endif