#ifndef __GL_EXAMPLE_SHADER_H__
#define __GL_EXAMPLE_SHADER_H__
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#elif __linux__
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

using namespace std;

class Shader{
private:
	unsigned int program_id;
public:
	void loadShader(const string& v_src, const string& f_src);
	void setMat4(const string &name, const glm::mat4 &mat);
	void setVec4(const string &name, const glm::vec4 &value);
	void setVec3(const string &name, const glm::vec3 &vec);
	void setVec3(const string &name, float x, float y, float z);
	void use();
	void setFloat(const string &name, float value);
	void setInt(const string &name, int value);
};

#endif