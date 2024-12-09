//Steven Bruns
//Went absolutely crazy trying to get a texture to place on the sphere.
//Had inpiration from Eric Weinbrenner's Examples. Thank you for having that cause I was going insane doing a texture.

#version 330 core
out vec4 FragColor;

in vec3 Pos;
in vec2 UV;

uniform float _Time;
uniform float sunSpeed;  // Set the Speed of the sun
uniform vec3 sunDir; // Uniform for sun direction

void main(){
    // Set the colors for the sky during the day and night
    vec3 sunsetCol = vec3(0.7, 0.4, 0.2);  // Sunset orange
    vec3 twilightCol = vec3(95.0/255.0, 84.0/255.0, 153.0/255.0); // Twilight purple

    // Adjust the sun's speed
    float adjustedTime = _Time * sunSpeed;  

    // Sun's vertical position controlled by adjustedTime
    float sunHeight = sin(adjustedTime); 

    // Mix the colors based on sun's height
    vec3 col = mix(twilightCol, sunsetCol, (sunHeight + 1.0) * 0.5); 

    // Simulate Direction
    //vec3 sunDir = normalize(vec3(sin(_Time), sunHeight, cos(_Time)));
    vec3 sunLightDir = normalize(sunDir); // Sun direction from uniform
    vec3 normal = normalize(Pos);

    // Simulate the sun's color and intensity
    float sunIntensity = max(dot(sunLightDir, normal), 0.0);
    sunIntensity = pow(sunIntensity, 128.0); // Sharp intensity effect

    // Sun color (orange/yellow)
    vec3 sunCol = vec3(1.0, 0.6, 0.2) * sunIntensity;

    // Add the sun color on top of the sky
    col += sunCol;

    col = clamp(col, 0.0, 1.0);

    // Final output
    FragColor = vec4(col, 1.0);
}
