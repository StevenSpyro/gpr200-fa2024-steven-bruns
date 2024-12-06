#version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec4 aColor;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec4 Color; //Varying
out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

uniform float uTime = 1.0;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0)); 
	vec3 pos = aPos;
	//Normal = mat3(transpose(inverse(model))) * aNormal;
	Normal = normalize(mat3(transpose(inverse(model))) * aNormal);
	TexCoords = aTexCoord;
	gl_Position = projection * view * model * vec4(aPos, 1.0);

}