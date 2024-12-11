#version 330 core // Brandon Cherry
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

    // Normalize wind direction
    vec3 normalWind = normalize(windDirection);

    // Calculate a weight factor based on height (This is so the bottom doesn't bend)
    float weight = clamp(aPos.y, 0.0, 1.0); 
    weight = pow(weight, 2.0); 

    // Bend
    float bend = weight * windSpeed;
    vec3 bendOffset = normalWind * bend;

    // Sway effect 
    float sway = weight * sin(aPos.y * 3.0 + _time * 2.0) * 0.7; //Magnitude of sway
    vec3 swayOffset = normalWind * sway;

    // Apply bending and swaying offsets
    vec4 worldPos = model * vec4(aPos + bendOffset + swayOffset, 1.0);

    // Add instance offset (translation)
    worldPos.x += aOffset.x;
    worldPos.z += aOffset.y;

    // Transform to clip space
    gl_Position = projection * view * worldPos;
}
