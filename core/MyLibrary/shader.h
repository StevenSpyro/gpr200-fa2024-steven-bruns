#pragma once
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"../ew/external/glad.h"

namespace myLibrary
{
	extern const char* vertexShaderSource;
	extern const char* fragmentShaderSource;

	class Shader
	{
	public:
		unsigned int ID;

		Shader(const char* vertexPath, const char* fragmentPath);

		void use();

		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setVec3(const std::string& name, const glm::vec3& value) const;
		void setVec3(const std::string& name, float x, float y, float z) const;
		void setMat4(const std::string& name, const glm::mat4& mat) const;
	};

}