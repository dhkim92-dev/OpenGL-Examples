#include <iostream>
#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#elif __linux__
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "glex.h"
// #include "../../external/stb_image.h"
using namespace std;

#define RENDER_COUNT 40

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
        -0.5f, -0.5f,  0.5f, -1.0f, -1.0,  1.0f,  0.0f,  0.0f,
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
	uint32_t pick_vao;

	vector<glm::mat4> models;

	App(string title, int width, int height)
		: BaseWindow::BaseWindow(title, width, height)
	{
	}

	void mouse_click_callback(GLFWwindow *window, int btn, int action, int mods) override {
		if(btn == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
			glFlush();
			glFinish();

			int id = 0;
			char data[4];
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			pickRender();
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			ypos = height - ypos;
			glReadPixels(xpos, ypos, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
			id = data[0] + data[1] * 255 + data[2] * 255;
			if(id == 0xFFFFFF00){
				cout << "cursor << (" << xpos << ", " << ypos << ")"<< endl;
				cout << "background picked\n";
			}else{
				cout << "cursor << (" << xpos << ", " << ypos << ")"<< endl;
				cout << "picking cube id : " << id << endl;
			}
		}
	}

private: 
	void preparePrograms() override {
		const string vtx_src = getShaderSource("../../shaders/pixelBasedObjectPicking/cube.vert");
		const string frag_src = getShaderSource("../../shaders/pixelBasedObjectPicking/cube.frag");
		
		const string pick_vtx_src = getShaderSource("../../shaders/pixelBasedObjectPicking/pick.vert");
		const string pick_frag_src = getShaderSource("../../shaders/pixelBasedObjectPicking/pick.frag");

		Shader cube, pick;
		cube.loadShader(vtx_src, frag_src);
		pick.loadShader(pick_vtx_src, pick_frag_src);
		progs["cube"] = cube;
		progs["pick"] = pick;
	}

	void prepareBuffers() override {

		//normal draw resources
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

		// pick draw resources
		glGenVertexArrays(1, &pick_vao);
		glBindVertexArray(pick_vao);
		glBindBuffer(GL_ARRAY_BUFFER,vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		models.resize(RENDER_COUNT);
		for(int i = 0 ; i < RENDER_COUNT ; i++){
			int z = 0;
			int y = i / 10 - 3;
			int x = i % 10 - 3;
			models[i] = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.1, 0.1, 0.1)), glm::vec3(x,y,z));
			cout << "models["<<i<<"] position : (" << x << ", " <<y << ", " << z << ")" << endl;
		}
	}

	void pickRender(){
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
		progs["pick"].use();
		glBindVertexArray(pick_vao);
		for(int i = 0 ; i < RENDER_COUNT  ; i++){
			progs["pick"].setMat4("model", models[i]);
			progs["pick"].setVec4("pixel", glm::vec4( i/255.0, 0, 0, 1.0f));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);
	}

	void render() override {
		progs["cube"].use();
		glBindVertexArray(vao);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);

		for(int i = 0 ; i < RENDER_COUNT ; i ++){
			progs["cube"].setMat4("model",models[i]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
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
		progs["cube"].use();
		progs["cube"].setMat4("view", view);
		progs["cube"].setMat4("proj", proj);
		progs["cube"].setInt("tex_sampler", 0);

		progs["pick"].use();
		progs["pick"].setMat4("view", view);
		progs["pick"].setMat4("proj", proj);

		current_frame_time = glfwGetTime();
		last_frame_time = glfwGetTime();

		while(!glfwWindowShouldClose(window))
		{
			glClearColor(1.0, 1.0, 1.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
			current_frame_time = glfwGetTime();
			render();
			last_frame_time = current_frame_time;
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glDeleteVertexArrays(1, &vao);
		glDeleteVertexArrays(1, &pick_vao);
		glDeleteBuffers(1, &vbo);

		glfwTerminate();
	}
};

int main(void)
{
	App app("Naive Pixel based Object Picking", 1600, 900);
	app.run();
	return 0;
}
