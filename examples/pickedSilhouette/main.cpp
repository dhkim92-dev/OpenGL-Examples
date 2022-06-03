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


struct Cube{
	const float vtx[48*6]={
		// positions          // normals           // texture coords
                -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	const glm::mat4 base = glm::scale(glm::mat4(1.0), glm::vec3(0.1, 0.1, 0.1));
	const glm::mat4 models[10] = {
		glm::translate(glm::rotate(base, glm::radians(20.0f), glm::vec3(0.3, 0.4, 0.2)) ,glm::vec3( 0.0f,  0.0f,  0.0f)),
		glm::translate(glm::rotate(base, glm::radians(40.0f), glm::vec3(0.3, 0.4, 0.2)) ,glm::vec3( 2.0f,  5.0f, -15.0f)),
		glm::translate(glm::rotate(base, glm::radians(60.0f), glm::vec3(0.3, 0.4, 0.2)) ,glm::vec3(-1.5f, -2.2f, -2.5f)),
		glm::translate(glm::rotate(base, glm::radians(80.0f), glm::vec3(0.3, 0.4, 0.2)) ,glm::vec3(-3.8f, -2.0f, -12.3f)),
		glm::translate(glm::rotate(base, glm::radians(100.0f), glm::vec3(0.3, 0.4, 0.2)) ,glm::vec3( 2.4f, -0.4f, -3.5f)),
		glm::translate(glm::rotate(base, glm::radians(120.0f), glm::vec3(0.3, 0.4, 0.2)) ,glm::vec3(-1.7f,  3.0f, -7.5f)),
		glm::translate(glm::rotate(base, glm::radians(140.0f), glm::vec3(0.3, 0.4, 0.2)) ,glm::vec3( 1.3f, -2.0f, -2.5f)),
		glm::translate(glm::rotate(base, glm::radians(160.0f), glm::vec3(0.3, 0.4, 0.2)) ,glm::vec3( 1.5f,  2.0f, -2.5f)),
		glm::translate(glm::rotate(base, glm::radians(180.0f), glm::vec3(0.3, 0.4, 0.2)) ,glm::vec3( 1.5f,  0.2f, -1.5f)),
		glm::translate(glm::rotate(base, glm::radians(200.0f), glm::vec3(0.3, 0.4, 0.2)) ,glm::vec3(-1.3f,  1.0f, -1.5f)) 
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
	uint32_t silhouette_vao;

	int picked_id = -1;

	Cube cube;
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
			drawPick();
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			ypos = height - ypos;
			glReadPixels(xpos, ypos, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
			id = data[0] + data[1] * 255 + data[2] * 255;
			if(id == -511){
				cout << "cursor << (" << xpos << ", " << ypos << ")"<< endl;
				cout << "background picked\n";
				picked_id = -1;
			}else{
				cout << "cursor << (" << xpos << ", " << ypos << ")"<< endl;
				cout << "picking cube id : " << id << endl;
				picked_id = id;
			}
		}
	}

private: 
	void preparePrograms() override {
		const string vtx_src = getShaderSource("../../shaders/pickedSilhouette/cube.vert");
		const string frag_src = getShaderSource("../../shaders/pickedSilhouette/cube.frag");
		
		const string pick_vtx_src = getShaderSource("../../shaders/pickedSilhouette/pick.vert");
		const string pick_frag_src = getShaderSource("../../shaders/pickedSilhouette/pick.frag");

		const string silhouette_vtx_src = getShaderSource("../../shaders/pickedSilhouette/silhouette.vert");
		const string silhouette_frag_src = getShaderSource("../../shaders/pickedSilhouette/silhouette.frag");
		Shader cube, pick, silhouette;
		cube.loadShader(vtx_src, frag_src);
		pick.loadShader(pick_vtx_src, pick_frag_src);
		silhouette.loadShader(silhouette_vtx_src, silhouette_frag_src);
		progs["cube"] = cube;
		progs["pick"] = pick;
		progs["silhouette"] = silhouette;
	}

	void prepareBuffers() override {

		//normal draw resources
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

		// silhouette
		glGenVertexArrays(1, &silhouette_vao);
		glBindVertexArray(silhouette_vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8 , (void *)0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
	}

	void drawPick(){
		glFlush();
		glFinish();
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);  

		progs["pick"].use();
		glBindVertexArray(pick_vao);
		for(int i = 0 ; i < 10 ; i++){
			progs["pick"].setMat4("model",cube.models[i]);
			progs["pick"].setVec4("pixel", glm::vec4(i/255.0, 0, 0, 1.0f));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);
	}

	void drawCubes(){
		// default renderpass to draw cubes
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		progs["cube"].use();
		glBindVertexArray(vao);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);

		for(int i = 0 ; i < 10 ; i ++){
			progs["cube"].setMat4("model", cube.models[i]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);
	}

	void drawSilhouette(){
		glBindVertexArray(silhouette_vao);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		progs["silhouette"].use();
		progs["silhouette"].setMat4("model", glm::scale(cube.models[picked_id], glm::vec3(1.1)));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);
	}

	void render() override {
		drawCubes();
		if(picked_id >= 0){
			drawSilhouette();
		}
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
	    glDepthFunc(GL_LESS);
	    glEnable(GL_STENCIL_TEST);
	    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

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
		progs["silhouette"].use();
		progs["silhouette"].setMat4("view", view);
		progs["silhouette"].setMat4("proj", proj);


		current_frame_time = glfwGetTime();
		last_frame_time = glfwGetTime();

		while(!glfwWindowShouldClose(window))
		{
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
	App app("Picked Object Silhouette Rendering", 1600, 900);
	app.run();
	return 0;
}
