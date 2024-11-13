#version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec4 aColor;
layout (location = 1) in vec2 aTexCoords;

out vec4 ourColor;
out vec2 TexCoords;

uniform float uTime = 1.0;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}