#ifndef __GL_EXAMPLE_BASE_WINDOW_CPP__
#define __GL_EXAMPLE_BASE_WINDOW_CPP__

#include <iostream>
#include "base_window.h"

using namespace std;

BaseWindow::BaseWindow(string title, int width, int height)  
: title(title), width(width), height(height)
{
	first_move = true;
	initGLFW();
}

void BaseWindow::initGLFW()
{
	//cout << "BaseWindow::InitGLFW() called" << endl;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
   	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
    #endif

	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

	if(!window){
		cerr << "fail to create GLFWwindow." << endl;
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSetCursorPos(window, width/2.0, height/2.0);
	glViewport(0, 0, width, height);
}

void BaseWindow::run()
{
	preparePrograms();
	prepareBuffers();
	while(!glfwWindowShouldClose(window))
	{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		current_frame_time = glfwGetTime();
		render();
		last_frame_time = current_frame_time;
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void BaseWindow::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	this->height = height;
	this->width = width;
	glViewport(0,0, width, height);
}

void BaseWindow::mouse_move_callback(GLFWwindow *window, double x, double y)
{

	if (first_move){
        lcursor[0] = x;
        lcursor[1] = y;
        first_move = false;
    }

    float xoffset = x - lcursor[0];
    float yoffset = y - lcursor[1];

    lcursor[0] = x;
    lcursor[1] = y;

	this->camera.mouseMoveHandler(xoffset, yoffset);
}

void BaseWindow::mouse_scroll_callback(GLFWwindow *window, double x, double y)
{
    camera.mouseScrollHandler(y);
}

void BaseWindow::mouse_click_callback(GLFWwindow *window, int btn, int action, int mods)
{
	cout << "mouse click callback : " << btn << ", " << action << " mods";
}

void BaseWindow::key_input_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	float delta = last_frame_time - current_frame_time;
	if(action == GLFW_PRESS){
		switch(key){
			case GLFW_KEY_W :
				camera.keyInputHandler(FORWARD, delta);
				break;
			case GLFW_KEY_S : 
				camera.keyInputHandler(BACKWARD, delta);
				break;
			case GLFW_KEY_A :
				camera.keyInputHandler(LEFT, delta);
				break;
			case GLFW_KEY_D :
				camera.keyInputHandler(RIGHT, delta);
				break;
			case GLFW_KEY_ESCAPE :
				glfwSetWindowShouldClose(window, 1);
				break;
		}
	}
}



#endif