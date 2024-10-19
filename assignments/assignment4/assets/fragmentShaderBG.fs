#version 330 core
out vec4 FragColor;
in vec4 ourColor;

in vec2 TexCoord;

// texture sampler
uniform sampler2D ourTexture;

uniform sampler2D texture1; //Box
uniform sampler2D texture2; //Face

uniform float uTime = 1.0;

void main()
{
	//vec2 tiledTexCoord1 = TexCoord * 1.0; 
    //vec2 tiledTexCoord2 = TexCoord * 1.0; 

    //vec4 texColor1 = texture(texture1, tiledTexCoord1);
    //vec4 texColor2 = texture(texture2, tiledTexCoord2);
    
    //FragColor = mix(texColor1, texColor2, 0.5);

    //Mix both textures.
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}