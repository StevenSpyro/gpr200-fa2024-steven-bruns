#version 330 core
out vec4 FragColor;
in vec4 ourColor;

in vec2 TexCoord;

// texture sampler
uniform sampler2D ourTexture;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float uTime = 1.0;

void main()
{
	vec4 texColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);

	FragColor = texColor;
}