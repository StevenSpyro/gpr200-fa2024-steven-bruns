// Fragment Shader Code
#version 330 core
out vec4 FragColor;

in vec3 FragPos;   
in vec3 Normal;    
in vec2 TexCoords; 

uniform sampler2D texture0;
uniform vec3 lightColor; // Light color from sun
uniform vec3 sunDir;     // Sun direction passed from skysphere shader
uniform vec3 viewPos;
uniform vec3 lightPos;

uniform float ambientK;     
uniform float diffuseK;     
uniform float specularK;    
uniform float shininess;    

void main() {
    // Ambient
    vec3 ambient = lightColor * ambientK;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(sunDir); // Use sunDir instead of recalculating
    //float diff = max(dot(norm, lightDir), 0.0);
    float diff = max(dot(normalize(norm), normalize(-sunDir)), 0.0);
    vec3 diffuse = diffuseK * diff * lightColor;

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    //vec3 reflectDir = reflect(-lightDir, norm);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    //vec3 specular = specularK * spec * lightColor;
    vec3 reflectDir = reflect(-lightDir, normalize(norm));
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = spec * lightColor * specularK;

    // Texture
    vec3 textureColor = texture(texture0, TexCoords).rgb;

    // Combine results
    vec3 result = (ambient + diffuse + specular) * textureColor;
    FragColor = vec4(result, 1.0);
}