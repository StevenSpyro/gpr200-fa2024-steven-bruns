#version 330 core
out vec4 FragColor;

in vec3 FragPos;   
in vec3 Normal;    
in vec2 TexCoords; 

uniform sampler2D texture0;
uniform vec3 lightColor;  // Light color 
uniform vec3 viewPos;     // Camera position 
uniform vec3 sunDir;      // Sun direction 

uniform float ambientK;   // Ambient 
uniform float diffuseK;   // Diffuse 
uniform float specularK;  // Specular 
uniform float shininess;  // Shininess 

void main()
{
    // Ambient Lighting 
    vec3 ambient = ambientK * lightColor;

    // Diffuse Lighting 
    vec3 norm = normalize(Normal); // Normalize 
    float diff = max(dot(norm, sunDir), 0.0); 
    vec3 diffuse = diffuseK * diff * lightColor; // Diffuse lighting

    // Specular Lighting 
    vec3 viewDir = normalize(viewPos - FragPos); 
    vec3 reflectDir = reflect(sunDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess); 
    vec3 specular = specularK * spec * lightColor; // Specular lighting

    // Texture color from the texture map
    vec3 textureColor = texture(texture0, TexCoords).rgb;

    // Combine results (ambient + diffuse + specular) and apply texture color
    vec3 result = (ambient + diffuse + specular) * textureColor;
    FragColor = vec4(result, 1.0); // Final color with combined lighting and texture
}