#version 450 core

layout (location = 0) in vec3	aPos;
layout (location = 1) in vec4	aColor;
layout (location = 2) in vec3	aNormal;
layout (location = 3) in vec2	aTexCoord;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 mvp;
uniform mat4 modelMat4;

void main()
{
	gl_Position = vec4(aPos, 1.0) * mvp;
	FragPos = vec3(vec4(aPos, 1.0) * modelMat4);
	
    TexCoord = aTexCoord;
	//Normal = (modelMat4 * vec4(aNormal, 0.0)).xyz;
    //Normal = aNormal;

   //calculation needed when model is not scaled uniformly
   Normal = mat3(transpose(inverse(modelMat4))) * aNormal;  
};