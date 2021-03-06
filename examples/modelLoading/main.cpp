#include <iostream>
#include <vector>
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
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace std;

class App : public BaseWindow
{
public : 
	unordered_map<string, Shader> progs;

	App(string title, int width, int height)
		: BaseWindow::BaseWindow(title, width, height)
	{
	}
	~App()
	{
		delete model;
	}

private: 
Model *model;

	void prepareCamera() override {
		camera = new QuatCamera();
		// camera->setPerspective(45.0f, width/(float)height, 0.1f, 100.0f);
	}
	void preparePrograms() override {
		const string vtx_src = getShaderSource("../../shaders/modelLoading/model.vert");
		const string frag_src = getShaderSource("../../shaders/modelLoading/model.frag");
		Shader shader;
		shader.loadShader(vtx_src, frag_src);
		progs["default"] = shader;
		cout << "preparePrograms done\n";
	}

	void prepareBuffers() override {
		model = new Model("../../resources/models/backpack/backpack.obj");
		cout << "buffer ready.\n";
	}

	void render() override {
		model->render(&progs["default"]);
	}


public:
	void run() {
		prepareCamera();
		preparePrograms();
		prepareBuffers();

		glEnable(GL_DEPTH_TEST);
		glFrontFace(GL_CCW);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		
		current_frame_time = glfwGetTime();
		last_frame_time = glfwGetTime();
		
		while(!glfwWindowShouldClose(window))
		{
			glClearColor(1.0, 1.0, 1.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
			current_frame_time = glfwGetTime();
			dt = current_frame_time - last_frame_time;
			progs["default"].use();
			progs["default"].setMat4("model", glm::mat4(1.0f));
			progs["default"].setMat4("projection", camera->getPerspective());
			progs["default"].setMat4("view", camera->getView());
			render();
			last_frame_time = current_frame_time;
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glfwTerminate();
	}
};

int main(void)
{
	App app("OpenGL Assimp Model loading", 1600, 900);
	app.run();
	return 0;
}
