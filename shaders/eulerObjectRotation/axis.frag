#version 410

in vec4 outColor;
out vec4 fragColor;

void main()
{
	fragColor = vec4(outColor.xyz, 0.0f) ;
}