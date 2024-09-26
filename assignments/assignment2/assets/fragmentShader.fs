#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture sampler
uniform sampler2D ourTexture;

uniform float uTime = 1.0;

void main()
{
	vec4 texColor = texture(ourTexture, TexCoord);

	FragColor = texColor;
}