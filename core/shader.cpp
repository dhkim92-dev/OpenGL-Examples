#ifndef __GL_EXAMPLE_SHADER_CPP__
#define __GL_EXAMPLE_SHADER_CPP__

#include <iostream>
#include "shader.h"

using namespace std;

void Shader::loadShader(const string& v_src, const string& f_src)
{
	unsigned int v_shader_id = glCreateShader(GL_VERTEX_SHADER);
	unsigned int f_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	GLint Result = GL_FALSE;
	int InfoLogLength;

	char const * v_shader_pointer = v_src.c_str();
	char const * f_shader_pointer = f_src.c_str();

	/*Vertex Shader Compile*/
	glShaderSource(v_shader_id, 1, &v_shader_pointer , NULL);
	glCompileShader(v_shader_id);

	glGetShaderiv(v_shader_id, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(v_shader_id, GL_INFO_LOG_LENGTH, &InfoLogLength);
	
	if ( InfoLogLength > 0 ){
		char vs_err[2048];
		glGetShaderInfoLog(v_shader_id, InfoLogLength, NULL, vs_err);
		cout << "Shader::loadShader::vertexShader error : " << vs_err << endl;
	}

	/*Fragment Shader Compile*/
	glShaderSource(f_shader_id, 1, &f_shader_pointer, NULL);
	glCompileShader(f_shader_id);

	glGetShaderiv(f_shader_id, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(f_shader_id, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if ( InfoLogLength > 0 ){
		char fs_error[2048];
		glGetShaderInfoLog(f_shader_id, InfoLogLength, NULL, fs_error);
		cout << "Shader::loadShader::fragmentShader error : " <<  fs_error << endl;
	}


	program_id = glCreateProgram();
	glAttachShader(program_id, v_shader_id);
	glAttachShader(program_id, f_shader_id);
	glLinkProgram(program_id);

	glGetProgramiv(program_id, GL_LINK_STATUS, &Result);
	glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		char prog_err[2048];
		glGetProgramInfoLog(program_id, InfoLogLength, NULL, prog_err);
		cout << "Shader::loadShader::program error : " << prog_err << endl;
	}

	glDetachShader(program_id, v_shader_id);
	glDetachShader(program_id, f_shader_id);

	glDeleteShader(v_shader_id);
	glDeleteShader(f_shader_id);
}

void Shader::use() 
{ 
    glUseProgram(program_id);
} 

void Shader::setMat4(const std::string &name, const glm::mat4 &mat)
{
    glUniformMatrix4fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
} 

void Shader::setVec4(const std::string &name, const glm::vec4 &value){
	glUniform4fv(glGetUniformLocation(program_id, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &vec)
{
	 glUniform3fv(glGetUniformLocation(program_id, name.c_str()), 1, &vec[0]);
}

void Shader::setVec3(const std::string &name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(program_id, name.c_str()), x, y, z); 
}

void Shader::setFloat(const std::string &name, float value)
{ 
    glUniform1f(glGetUniformLocation(program_id, name.c_str()), value); 
}

void Shader::setInt(const std::string &name, int value){
	glUniform1i(glGetUniformLocation(program_id, name.c_str()),value);
}

#endif