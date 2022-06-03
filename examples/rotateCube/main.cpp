#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#elif __linux__
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#include "glex.h"
#include <GLFW/glfw3.h>

using namespace std;

class App : public BaseWindow
{
public : 
	unordered_map<string, Shader> progs;
	// unordered_map<string, Mesh> meshes;

	uint32_t vao;
	uint32_t vbo;
	uint32_t ibo;

	App(string title, int width, int height)
		: BaseWindow::BaseWindow(title, width, height)
	{
	}
private: 
	void preparePrograms() override {
		const string vtx_src = getShaderSource("../../shaders/rotateCube/cube.vert");
		const string frag_src = getShaderSource("../../shaders/rotateCube/cube.frag");
		Shader cube;
		cube.loadShader(vtx_src, frag_src);
		progs["cube"] = cube;
	}

	void prepareBuffers() override {
		const float vtx[8][6]{
			// x, y, z , r , g, b
			{-0.5, -0.5, 0.5, 1.00, 0.00, 0.00}, // v0
			{0.5, -0.5, 0.5, 0.75, 0.25, 0.00}, //v1
			{0.5,  0.5, 0.5, 0.50, 0.50, 0.00}, //v2
			{-0.5,  0.5, 0.5, 0.25, 0.75, 0.00}, //v3
			{-0.5, -0.5,-0.5, 0.00, 1.00, 0.00}, //v4
			{0.5, -0.5,-0.5, 0.00, 0.75, 0.25}, //v5
			{0.5,  0.5,-0.5, 0.00, 0.50, 0.50},//v6
			{-0.5,  0.5,-0.5, 0.00, 0.25, 0.75}//v7
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
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 36*sizeof(uint32_t), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	public:
	void run() {
		prepareCamera();
		prepareBuffers();
		preparePrograms();

		//glEnable(GL_DEPTH_TEST);
		glFrontFace(GL_CCW);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glm::mat4 view = camera->getView();
		glm::mat4 proj = glm::perspective(
			glm::radians(45.0f),
			(float)width/(float)height,
			0.1f,
			100.f
		);
		glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(0.1, 0.1, 0.1));

		progs["cube"].use();
		progs["cube"].setMat4("model", model);
		progs["cube"].setMat4("view", view);
		progs["cube"].setMat4("proj", proj);
		current_frame_time = glfwGetTime();
		last_frame_time = glfwGetTime();
		while(!glfwWindowShouldClose(window))
		{
			glClearColor(1.0, 1.0, 1.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
			current_frame_time = glfwGetTime();
			float delta = current_frame_time - last_frame_time;
			model = glm::rotate(model, 1.0f * delta, glm::vec3(0, 1, 0));
			progs["cube"].setMat4("model", model);
			render();
			last_frame_time = current_frame_time;
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ibo);

		glfwTerminate();
	}
};

int main(void)
{
	App app("OpenGL Rotate Cube Example", 1600, 900);
	app.run();
	return 0;
}
