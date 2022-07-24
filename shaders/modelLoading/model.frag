#version 410

out vec4 outColor;
in vec2 outUV;
uniform sampler2D diffuse_tex;

void main()
{    
    outColor = texture(diffuse_tex, outUV);
}