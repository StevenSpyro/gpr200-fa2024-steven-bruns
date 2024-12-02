#version 330 core
out vec4 FragColor;
in vec3 TexCoords;

//uniform samplerCube skybox; //Skybox
uniform vec3 skyColor; //Sky Color
uniform sampler2D skyTexture; //Black and White Gradient

void main()
{
    float u = 0.5 + atan(TexCoords.z, TexCoords.x) / (2.0 * 3.141592653589793);
    float v = 0.5 - asin(TexCoords.y) / 3.141592653589793; 

    float brightness = texture(skyTexture, vec2(u,v)).r; //Needed for the Black and White image
    vec3 color = mix(vec3(0.0), skyColor, brightness); //Grayscale can be color.
    FragColor = vec4(color, 1.0);
}


/*
in vec3 texCoords;

uniform samplerCube skybox;

void main()
{
    FragColor = texture(skybox, texCoords);
}
*/

//FragColor = vec4(color, 1.0);