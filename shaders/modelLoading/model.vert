#version 410

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;

out vec2 outUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    outUV = inUV;
    gl_Position = projection * view * model * vec4(inPos, 1.0);
}