#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <unordered_map>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#elif __linux__
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION 1
#include "glex.h"

using namespace std;

class App : public BaseWindow
{
public : 
	unordered_map<string, Shader> progs;

	uint32_t vao, vbo, ibo;

	App(string title, int width, int height)
		: BaseWindow::BaseWindow(title, width, height)
	{
	}
private: 
	void preparePrograms() override {
		const string vtx_src = getShaderSource("../../shaders/cube/cube.vert");
		const string frag_src = getShaderSource("../../shaders/cube/cube.frag");
		Shader cube;
		cube.loadShader(vtx_src, frag_src);
		progs["cube"] = cube;
	}

	void prepareBuffers() override {
		const float vtx[8][6]={
			// x, y, z
			{-0.5, -0.5, 0.5, 1.00, 0.00, 0.00}, // v0
			{0.5, -0.5, 0.5, 0.75, 0.25, 0.00}, //v1
			{0.5,  0.5, 0.5, 0.50, 0.50, 0.00}, //v2
			{-0.5,  0.5, 0.5, 0.25, 0.75, 0.00}, //v3
			{-0.5, -0.5,-0.5, 0.00, 1.00, 0.00}, //v4
			{0.5, -0.5,-0.5, 0.00, 0.75, 0.25}, //v5
			{0.5,  0.5,-0.5, 0.00, 0.50, 0.50},//v6
		};

		const uint32_t idx[36] = {
			1, 2, 3, // 0
			1, 3, 0, // 1
			2, 1, 5, // 2
			2, 5, 6, // 3
			6, 5, 4, // 4
			6, 4, 7, // 5
			3, 7, 4, // 6
			3, 4, 0, // 7
			0, 5, 1,// 8
			0, 4, 5,// 9
			3, 2, 6,// 10
			3, 6, 7// 11
		};

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ibo);
		
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 8, vtx, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * 36, idx, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*)(sizeof(float)*3));
		glBindVertexArray(0);
	}

	void render() override {
		progs["cube"].use();
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, sizeof(uint32_t) * 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
};

int main(void)
{
	App app("OpenGL IBO Cube", 1600, 900);
	app.run();
	return 0;
}
