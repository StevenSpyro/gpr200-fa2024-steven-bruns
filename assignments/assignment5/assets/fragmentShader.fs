// Fragment Shader Code
#version 330 core
out vec4 FragColor;

in vec3 FragPos;   
in vec3 Normal;    
in vec2 TexCoords; 

uniform sampler2D texture0;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float ambientK;     
uniform float diffuseK;     
uniform float specularK;    
uniform float shininess;    

void main()
{
    // Ambient
    vec3 ambient = ambientK * lightColor;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuseK * diff * lightColor;

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularK * spec * lightColor;

    // Texture
    vec3 textureColor = texture(texture0, TexCoords).rgb;

    // Combine results
    vec3 result = (ambient + diffuse + specular) * textureColor;
    FragColor = vec4(result, 1.0);
}