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

BaseWindow::~BaseWindow()
{
	glfwDestroyWindow(window);
	window = nullptr;
	delete camera;
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

	glfwSetWindowUserPointer(window, this);

	auto frameBufferSizeCallback = [](GLFWwindow *_window, int _width, int _height){
		static_cast<BaseWindow *>(glfwGetWindowUserPointer(_window))->framebuffer_size_callback(_window, _width, _height);
	};

	auto mouseCursorPosCallback = [](GLFWwindow *_window, double x, double y){
		static_cast<BaseWindow *>(glfwGetWindowUserPointer(_window))->mouse_move_callback(_window, x, y);
	};

	auto mouseScrollCallback = [](GLFWwindow *_window, double x, double y){
		static_cast<BaseWindow *>(glfwGetWindowUserPointer(_window))->mouse_scroll_callback(_window, x, y);
	};

	auto mouseClickCallback = [](GLFWwindow *_window, int btn, int action, int mods){
		static_cast<BaseWindow *>(glfwGetWindowUserPointer(_window))->mouse_click_callback(_window, btn, action, mods);
	};

	auto keyCallback = [](GLFWwindow *_window, int key, int scancode, int action, int mods){
		static_cast<BaseWindow *>(glfwGetWindowUserPointer(_window))->key_input_callback(_window, key, scancode, action, mods);
	};


	glfwSetCursorPosCallback(window, mouseCursorPosCallback);
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
	glfwSetScrollCallback(window, mouseScrollCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseClickCallback);
}

void BaseWindow::prepare()
{
	prepareCamera();
	prepareBuffers();
	preparePrograms();
	
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void BaseWindow::prepareCamera()
{
	camera = new ECamera();
}

void BaseWindow::run()
{
	prepare();
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
	glfwTerminate();
}

void BaseWindow::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	// cout << "framebuffer size callback : " << width << ", " << height << endl;
	this->height = height;
	this->width = width;
	camera->setPerspective(45.0f, (float)width/(float)height, 0.1f, 100.0f);
	glViewport(0,0, width, height);
}

void BaseWindow::mouse_move_callback(GLFWwindow *window, double x, double y)
{
	// cout << "mouse move callback : " << x << ", " << y << endl;
	if (first_move){
        lcursor[0] = x;
        lcursor[1] = y;
        first_move = false;
    }

    float xoffset = x - lcursor[0];
    float yoffset = y - lcursor[1];

    lcursor[0] = x;
    lcursor[1] = y;

	this->camera->mouseMoveHandler(xoffset, yoffset);
}

void BaseWindow::mouse_scroll_callback(GLFWwindow *window, double x, double y)
{
    camera->mouseScrollHandler(y);
}

void BaseWindow::mouse_click_callback(GLFWwindow *window, int btn, int action, int mods)
{
	// cout << "mouse click callback : " << btn << ", " << action << " mods " << endl;;
}

void BaseWindow::key_input_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// cout << "key : " << key << " scan code : " << scancode << endl;
	float delta = dt;
	// cout << "current frame time : " << current_frame_time << endl;
	// cout << "last frame time : " << last_frame_time << endl;
	if(action == GLFW_PRESS || action == GLFW_REPEAT){
		switch(key){
			case GLFW_KEY_W :
				camera->keyInputHandler(FORWARD, delta);
				break;
			case GLFW_KEY_S : 
				camera->keyInputHandler(BACKWARD, delta);
				break;
			case GLFW_KEY_A :
				camera->keyInputHandler(LEFT, delta);
				break;
			case GLFW_KEY_D :
				camera->keyInputHandler(RIGHT, delta);
				break;
			case GLFW_KEY_ESCAPE :
				glfwSetWindowShouldClose(window, 1);
				break;
		}
	}
}

GLFWwindow* BaseWindow::getWindow()
{
	return window;
}

#endif