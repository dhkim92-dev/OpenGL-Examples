#version 410

// in vec3 frag_normal;
in vec2 frag_uv;
out vec4 outColor;
uniform sampler2D tex_sampler;

void main()
{
	// vec3 pixel = texture(tex_sampler, frag_uv).rgb;
	// outColor = vec4(normalize(frag_normal), 1.0f);
	 outColor = texture(tex_sampler, frag_uv);
}