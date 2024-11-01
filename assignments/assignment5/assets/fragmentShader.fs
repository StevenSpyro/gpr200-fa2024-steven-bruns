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


/*

in vec2 TexCoord;

// texture samples
uniform sampler2D texture0; //Water

//New Uniforms
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
//uniform vec3 objectColor;

uniform float ambientK;
uniform float diffuseK;
uniform float specularK;
uniform float shininess;

void main()
{
	//Ambient
	float ambientK = 0.1;
	vec3 ambient = ambientK * lightColor;

	//Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diffuseK * diff * lightColor;

	//Specular

	float spec = 0.0;
	if(blinn)
	{
		vec3 halfwayDir = normalize(lightDir + viewDir);
		spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
	}
	else
	{
		vec3 reflectDir = reflect(-lightDir, normal);
		spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	}
	vec3 specular = vec3(0.3) * spec;

	vec3 textureColor = texture(texture0, TexCoord).rgb;

	vec3 result = (ambient + diffuse + specular) * textureColor;
    FragColor = vec4(result, 1.0);

	//Mix both textures.
	//FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.5);
}

*/

/*
void main()
{
	vec3 color = texture(texture0, fs_in.TexCoord).rgb;

	//ambient

	float ambientK = 0.1;
	vec3 ambient = ambientK * lightColor;

	//Diffuse
	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	vec3 normal = normalize(fs_in.Normal);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * color;

	//Specular
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = 0.0;
	if(blinn)
	{
		vec3 halfwayDir = normalize(lightDir + viewDir);
		spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
	}
	else
	{
		vec3 reflectDir = reflect(-lightDir, normal);
		spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	}
	vec3 specular = vec3(0.3) * spec;
	vec3 result = (ambient + diffuse + specular) * color;
    FragColor = vec4(result, 1.0);


}
*/