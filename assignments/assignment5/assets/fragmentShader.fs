#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samples
uniform sampler2D texture0; //Water
uniform sampler2D texture1; //Link

void main()
{
	//Mix both textures.
	FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.5);
}