#version 410

// in vec3 frag_normal;
layout (location=0) in vec3 inPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;


void main()
{
	// vec3 pixel = texture(tex_sampler, frag_uv).rgb;
	// outColor = vec4(normalize(frag_normal), 1.0f);
	gl_Position = proj*view*model*vec4(inPos, 1.0f);
}