#version 450 core

out vec4 FragColor;

uniform vec3 colliderColor;
uniform bool colliderVisible;

void main()
{
    FragColor = vec4(colliderColor, 1.0);
};

