//Steven Bruns
//Went absolutely crazy trying to get a texture to place on the sphere.
//Had inpiration from Eric Weinbrenner's Examples. Thank you for having that cause I was going insane doing a texture.

#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

uniform mat4 _Model;
uniform mat4 _ViewProjection;

out vec3 Pos;
out vec2 UV;
void main(){
	Pos = aPos;
	UV = aUV;
	gl_Position =_ViewProjection * _Model * vec4(aPos,1.0);
	gl_Position = gl_Position.xyww;
}

