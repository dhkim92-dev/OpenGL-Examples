#version 410

layout(location=0) in vec3 inPos;
// layout(location=1) in vec3 inColor;
out vec4 outColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	gl_Position = proj*view*model*vec4(inPos, 1.0);

	outColor = vec4(normalize(abs(inPos)), 1.0f);
}