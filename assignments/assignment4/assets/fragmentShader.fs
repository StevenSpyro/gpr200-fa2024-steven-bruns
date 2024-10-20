#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samples
uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
	//Mix both textures.
	FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.5);
}