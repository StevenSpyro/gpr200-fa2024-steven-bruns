#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} fs_in;

uniform sampler2D texture0;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform bool blinn;
uniform float shininess;
uniform float ambientK;
uniform float diffuseK; 

void main()
{
    // Ambient
    vec3 ambient = ambientK * lightColor;

    // Diffuse
    vec3 norm = normalize(fs_in.Normal); 
    vec3 lightDir = normalize(lightPos - fs_in.FragPos); 
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuseK * diff * lightColor; 

    // Specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    float spec = 0.0;
    if(blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, norm);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    }
    
    vec3 specular = vec3(0.3) * spec;

    vec3 textureColor = texture(texture0, fs_in.TexCoord).rgb;

    vec3 result = (ambient + diffuse + specular) * textureColor;
    FragColor = vec4(result, 1.0);
}

