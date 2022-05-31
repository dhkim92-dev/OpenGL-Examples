#version 410

layout (location=0) in vec3 pos;
layout (location=1) in vec3 normal;
out vec3 inColor;

void main(void)
{
	gl_Position = vec4(pos, 1.0);
	inColor = normal;
}