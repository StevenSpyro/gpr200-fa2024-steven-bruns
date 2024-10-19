#version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec4 aColor;
layout (location = 1) in vec2 aTexCoord;

out vec4 ourColor;
out vec2 TexCoord;

uniform float uTime = 1.0;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	//vec3 pos = aPos;

	//gl_Position = vec4(aPos, 1.0);
	//ourColor = aColor;
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}