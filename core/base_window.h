#ifndef __GL_EXAMPLE_BASE_WINDOW_H__
#define __GL_EXAMPLE_BASE_WINDOW_H__

#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#elif __linux__
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#endif
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
	BaseCamera* camera;
	float last_frame_time;
	float current_frame_time;
	float dt;

	void initGLFW();
	virtual void preparePrograms()=0;
	virtual void prepareBuffers()=0;
	virtual void prepareCamera();
	virtual void render()=0;

public:
	BaseWindow(string title, int width, int height);
	~BaseWindow();
	virtual void run();	
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	virtual void prepare();
	virtual void mouse_move_callback(GLFWwindow* window, double x, double y);
	virtual void mouse_scroll_callback(GLFWwindow* window, double x, double y);
	virtual void mouse_click_callback(GLFWwindow *window, int btn, int action, int mods);
	virtual void key_input_callback(GLFWwindow* window, int key, int scancode, int action,int mods);
	GLFWwindow *getWindow();
};

#endif