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
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION 1
#include "glex.h"
#include "stb_image.h"
using namespace std;

struct Cube{
	const float vtx[48*6]={
		//x     y      z      nx      ny    nz      u      v
	    -0.5f, -0.5f, -0.5f,  -1.0f,  -1.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  -1.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,
    	-0.5f,  0.5f, -0.5f, -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  -1.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  1.0f,  1.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, -1.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, -1.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  1.0f,  1.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  -1.0f, -1.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  -1.0f, -1.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  -1.0f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f, -1.0f, -1.0f, -1.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, -1.0f,  1.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, -1.0f,  1.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, -1.0f, -1.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f, -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  1.0f, -1.0f,  0.0f,  1.0f
	};
};

class App : public BaseWindow
{
public : 
	unordered_map<string, Shader> progs;
	uint32_t vao;
	uint32_t vbo;
	uint32_t tex;

	App(string title, int width, int height)
		: BaseWindow::BaseWindow(title, width, height)
	{
	}

private: 
	void preparePrograms() override {
		const string vtx_src = getShaderSource("../../shaders/textureCube/cube.vert");
		const string frag_src = getShaderSource("../../shaders/textureCube/cube.frag");
		Shader cube;
		cube.loadShader(vtx_src, frag_src);
		progs["cube"] = cube;
	}

	void prepareBuffers() override {
		Cube cube;
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube.vtx), cube.vtx, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)12);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)24);
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
	}

	void render() override {
		glBindVertexArray(vao);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

	void prepareTexture()
	{
		glGenTextures(1, &tex);
		int tex_w, tex_h, nr_comps;
		GLenum format;
		uint8_t *data = stbi_load("../../resources/textures/cube/container2.png", &tex_w, &tex_h, &nr_comps, 0);
		if(nr_comps == 1){
				cout << "format RED\n";
				format = GL_RED;
		}else if(nr_comps == 3){
				cout << "format RGB\n";
				format = GL_RGB;
		}else if(nr_comps == 4){
			cout << "format RGBA\n";
			format = GL_RGBA;
		}

		cout << "texture load width : " << tex_w << " height : " << tex_h << " channels " << nr_comps << endl;
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexImage2D(GL_TEXTURE_2D, 0, format, tex_w, tex_h, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(data);
	}

	public:
	void run() {
		prepareCamera();
		preparePrograms();
		prepareBuffers();
		prepareTexture();

		glEnable(GL_DEPTH_TEST);
		glFrontFace(GL_CCW);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

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
		progs["cube"].setInt("tex_sampler", 0);

		current_frame_time = glfwGetTime();
		last_frame_time = glfwGetTime();
		while(!glfwWindowShouldClose(window))
		{
			glClearColor(1.0, 1.0, 1.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
			current_frame_time = glfwGetTime();

			for(int i = 0 ; i < 4 ; i++){
				glm::mat4 _model = glm::translate(model, glm::vec3(i*2.0, 0, 0));
				progs["cube"].setMat4("model", _model);
				render();
			}
			last_frame_time = current_frame_time;
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);

		glfwTerminate();
	}
};

int main(void)
{
	App app("OpenGL Multiple Cube Draw Example", 1600, 900);
	app.run();
	return 0;
}
