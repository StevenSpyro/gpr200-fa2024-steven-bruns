#include "shader.h"

namespace shader
{
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec4 aColor;\n"
		"out vec4 Color;\n" //Varying
		"uniform float uTime;\n"
		"void main()\n"
		"{\n"
		"   Color = aColor;\n" //Pass-Through
		"   vec3 pos = aPos;\n"
		"   pos.y += sin(uTime + pos.x) / 4;\n"
		"   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);\n"
		"}\0";
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec4 Color;\n"
		"uniform float uTime;\n"
		"uniform vec4 uColor = vec4(1.0);\n"
		"void main()\n"
		"{\n"
		"   FragColor = Color * (sin(uTime) * 0.5 + 0.5);\n"//vec4(1.0f, 0.5f, 0.2f, 1.0f);
		"}\n\0";
}

