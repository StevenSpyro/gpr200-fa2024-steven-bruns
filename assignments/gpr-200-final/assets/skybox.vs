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

/*
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
*/


/*
out vec3 texCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	vec4 pos = projection * view * vec4(aPos, 1.0f);
	gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
	texCoords = vec3(aPos.x, aPos.y, -aPos.z);
}

void main()
{
    TexCoords = aPos; 
    gl_Position = projection * view * vec4(aPos, 1.0); 
}

#version 330 core
layout (location = 0) in vec3 aPos;
out vec3 TexCoords;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	TexCoords = normalize(aPos); //Sphere and texture coordinates are the same.
	vec4 pos = projection * view * vec4 (aPos, 1.0);
	gl_Position = pos.xyww; //Skybox in back
}


*/