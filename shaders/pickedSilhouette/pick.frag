#version 410

// in vec3 frag_normal;
uniform vec4 pixel;
out vec4 fragColor;

void main()
{
	fragColor = pixel;
}