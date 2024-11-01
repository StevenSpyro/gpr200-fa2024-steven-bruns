#version 330 core
out vec4 FragColor;
in vec4 ourColor;


in vec2 TexCoord;

// texture sampler
uniform sampler2D ourTexture;

uniform sampler2D texture1; //Water

uniform vec3 lightColor;

uniform float uTime = 1.0;

void main()
{

	FragColor = vec4(lightColor, 1.0);
}