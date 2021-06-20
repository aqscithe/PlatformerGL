#version 450 core

layout (location = 0) in vec3	aPos;
layout (location = 1) in vec4	aColor;
layout (location = 2) in vec3	aNormal;
layout (location = 3) in vec2	aTexCoord;

uniform mat4 mvp;

void main()
{
	gl_Position = vec4(aPos, 1.0) * mvp ;
};