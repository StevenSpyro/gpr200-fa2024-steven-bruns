#version 330 core //Brandon Cherry
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec2 aOffset;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;



void main()
{
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos.x + aOffset.x, aPos.y, aPos.z + aOffset.y, 1.0);
}