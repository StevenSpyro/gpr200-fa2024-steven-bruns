#version 330 core
out vec4 FragColor;
in vec4 ourColor;

in vec2 TexCoord;

// texture sampler
uniform sampler2D ourTexture;

uniform sampler2D texture1; //Water
uniform sampler2D texture2; //Link

uniform float uTime = 1.0;

void main()
{
    //Mix both textures.
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}