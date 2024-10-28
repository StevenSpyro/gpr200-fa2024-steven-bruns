#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

in vec2 TexCoord;

// texture samples
uniform sampler2D texture0; //Water
uniform sampler2D texture1; //Link

//New Uniforms
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
//uniform vec3 objectColor;

void main()
{
	//Ambient
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	//Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	//Specular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 textureColor = texture(texture0, TexCoord).rgb;
	vec3 result = (ambient + diffuse + specular) * textureColor;
	//vec3 result = (ambient + diffuse + specular) * texture0;
    FragColor = vec4(result, 1.0);

	//Mix both textures.
	//FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.5);
}