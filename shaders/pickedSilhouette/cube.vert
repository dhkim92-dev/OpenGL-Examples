#version 410

layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 uv;
// out vec3 frag_normal;
out vec2 frag_uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
//	frag_pos = vec3(model * vec4(pos, 1.0f));
	// frag_normal = mat3(transpose(inverse(model))) * normal;
	gl_Position = proj * view * model * vec4(pos, 1.0f);
	frag_uv = uv;
}