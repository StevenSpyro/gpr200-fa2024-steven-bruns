#version 330 core //Brandon Cherry
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec2 aOffset;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float _time;
uniform float windSpeed;
uniform vec3 windDirection;



void main()
{
    TexCoords = aTexCoords;

    
    vec3 normalWind = normalize(windDirection);
    
    float bend = aPos.y * windSpeed;
    vec3 bendOffset = normalWind * bend;

    float sway = sin(aPos.y * 3.0 + _time * 2.0) * 0.1;
    vec3 swayOffset = normalWind * sway;

    vec4 worldPos = model * vec4(aPos + bendOffset + swayOffset, 1.0);
    worldPos.x += aOffset.x;
    worldPos.z += aOffset.y;

    
    gl_Position = projection * view * worldPos;

   
}