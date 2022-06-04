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

struct Cube{
const float vtx[8][6]={
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

	uint32_t vao, vbo, ebo;
	float angleX=0.0, angleY=0.0, angleZ=0.0;
	float sensitivity = 0.2;
	glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(0.4));

	glm::mat4 rotate(){
		return model;
	}

	~Cube(){
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
	}
};

struct Axis{
	const float vtx[6][3]={
		{-1.0f, 0.0f, 0.0f},
		{1.0f, 0.0f, 0.0f},
		{0.0f, -1.0f, 0.0f},
		{.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, -1.0f},
		{0.0f, 0.0f, 1.0f}
	};
	const uint32_t idx[3][2]={
		{0,1},
		{2,3},
		{4,5}
	};

	uint32_t vao, vbo, ebo;
	~Axis(){
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
	}
};

class App : public BaseWindow
{
public : 
	unordered_map<string, Shader> progs;

	Cube cube;
	Axis axis, cube_axis;
	int selected_axis = -1;
	double cX, cY;
	double last_cX, last_cY;

	App(string title, int width, int height)
		: BaseWindow::BaseWindow(title, width, height)
	{
	}

	void key_input_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if(key == GLFW_KEY_X && action == GLFW_REPEAT){
			selected_axis = 0;
		}else if(key ==GLFW_KEY_Y && action == GLFW_REPEAT){
			selected_axis = 1;
		}else if(key == GLFW_KEY_Z && action == GLFW_REPEAT){
			selected_axis = 2;
		}else{
			selected_axis = -1;
			BaseWindow::key_input_callback(window, key, scancode, action, mods);
		} 	
	}

	void mouse_move_callback(GLFWwindow *window, double x, double y)
	{
		double dx = last_cX - x;
		double dy = last_cY - y;
		double delta = sqrt( dx*dx + dy*dy ) * cube.sensitivity * 0.0001;
		switch(selected_axis){
			case 0 :
				cube.angleX += delta;
				if(cube.angleX > 360.f) cube.angleX -= 360.f;
				if(cube.angleX < 0.0f) cube.angleX = 360.f + cube.angleX;
				cube.model = glm::rotate(cube.model,(float)delta, glm::vec3(1.0f, 0.0f, 0.0f));
				break;
			case 1 :
				cube.angleY += delta;
				if(cube.angleY > 360.f) cube.angleY -= 360.f;
				if(cube.angleY < 0.0f) cube.angleY = 360.f + cube.angleY;
				cube.model = glm::rotate(cube.model,(float)delta, glm::vec3(0.0f, 1.0f, 0.0f));
				break;
			case 2 :
				cube.angleZ += delta;
				if(cube.angleZ > 360.f) cube.angleZ -= 360.f;
				if(cube.angleZ < 0.0f) cube.angleZ = 360.f + cube.angleZ;
				cube.model = glm::rotate(cube.model,(float)delta, glm::vec3(0.0f, 0.0f, 1.0f));
				break;
			default :
				// cout << "default mouse callback\n";
				// BaseWindow::mouse_move_callback(window, x, y);
				break;
		}
	}

private: 
	void prepareCamera() override {
		camera = new QuatCamera(glm::vec3(2.0, 0.0, 1.0), -60.0, 0);
		// camera = new QuatCamera();
		camera->setPerspective(45.0f, (double)width/(double)height, 0.1f, 100.0f);
	}

	void preparePrograms() override {
		const string vtx_src = getShaderSource("../../shaders/eulerObjectRotation/cube.vert");
		const string frag_src = getShaderSource("../../shaders/eulerObjectRotation/cube.frag");
		const string axis_vert = getShaderSource("../../shaders/eulerObjectRotation/axis.vert");
		const string axis_frag = getShaderSource("../../shaders/eulerObjectRotation/axis.frag");

		Shader cube_shader, axis_shader;
		cube_shader.loadShader(vtx_src, frag_src);
		axis_shader.loadShader(axis_vert, axis_frag);
		progs["cube"] = cube_shader;
		progs["axis"] = axis_shader;
	}

	void prepareBuffers() override {
		//Cube
		glGenVertexArrays(1, &cube.vao);
		glGenBuffers(1, &cube.vbo);
		glGenBuffers(1, &cube.ebo);
		
		glBindVertexArray(cube.vao);
		glBindBuffer(GL_ARRAY_BUFFER, cube.vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube.vtx), cube.vtx, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube.ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube.idx), cube.idx, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*)(sizeof(float)*3));
		glBindVertexArray(0);

		//CubeAxis
		glGenVertexArrays(1, &cube_axis.vao);
		glGenBuffers(1, &cube_axis.vbo);
		glGenBuffers(1, &cube_axis.ebo);

		glBindVertexArray(cube_axis.vao);
		glBindBuffer(GL_ARRAY_BUFFER, cube_axis.vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_axis.vtx), cube_axis.vtx, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_axis.ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_axis.idx), cube_axis.idx, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, (void*)0);
		glBindVertexArray(0);

	}

	void drawCubeAxis(){
		Shader &prog = progs["axis"];
		prog.use();
		prog.setMat4("model", glm::scale(cube.rotate(), glm::vec3(4.0)));
		prog.setMat4("view", camera->getView());
		prog.setMat4("proj", camera->getPerspective());
		glBindVertexArray(cube_axis.vao);
		glLineWidth(0.1f);
		glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);
	}

	void drawCube(){
		// glPolygonMode(GL_FRONT, GL_TRIANGLES);
		Shader &prog = progs["cube"];
		prog.use();
		prog.setMat4("model", cube.rotate());
		prog.setMat4("view", camera->getView());
		prog.setMat4("proj", camera->getPerspective());
		glBindVertexArray(cube.vao);
		glDrawElements(GL_TRIANGLES, sizeof(uint32_t) * 36, GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);
	}

	void render() override {
		drawCube();
		drawCubeAxis();
	}

	public :
	void run() override {
		prepare();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		current_frame_time = glfwGetTime();
		last_frame_time = glfwGetTime();
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		while(!glfwWindowShouldClose(window))
		{
			glClearColor(1.0, 1.0, 1.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
			current_frame_time = glfwGetTime();
			dt = current_frame_time - last_frame_time;
			last_frame_time = current_frame_time;
			render();
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glfwTerminate();
	}
};

int main(void)
{	
	App app("Euler Object Rotation", 1600, 900);
	app.run();
	return 0;
}
