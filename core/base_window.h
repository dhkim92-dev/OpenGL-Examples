#ifndef __GL_EXAMPLE_BASE_WINDOW_H__
#define __GL_EXAMPLE_BASE_WINDOW_H__

#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "camera.h"
#include "shader.h"

using namespace std;

class BaseWindow{
protected:
	GLFWwindow *window;
	int width, height;
	float lcursor[2];
	bool first_move;
	const string title;
	ECamera camera;
	float last_frame_time;
	float current_frame_time;

	// void (*framebuffer_size_cb)(GLFWwindow*, int ,int) = nullptr;
	// void (*mouse_move_cb)(GLFWwindow*, double, double) = nullptr;
	// void (*mouse_click_cb)(GLFWwindow*, int, int ,int) = nullptr;
	// void (*mouse_scroll_cb)(GLFWwindow*, double, double) = nullptr;
	// void (*key_input_cb)(GLFWwindow*, int, int, int ,int) = nullptr;

	void initGLFW();
	virtual void preparePrograms()=0;
	virtual void prepareBuffers()=0;
	virtual void render()=0;

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	virtual void mouse_move_callback(GLFWwindow* window, double x, double y);
	virtual void mouse_scroll_callback(GLFWwindow* window, double x, double y);
	virtual void mouse_click_callback(GLFWwindow *window, int btn, int action, int mods);
	virtual void key_input_callback(GLFWwindow* window, int key, int scancode, int action,int mods);
public:
	BaseWindow(string title, int width, int height);
	virtual void run();	
};

#endif