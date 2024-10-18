#include "texture2D.h"

using namespace std;

namespace myLibrary
{
	Texture2D::Texture2D(const char* filePath, int filterMode, int wrapMode)
	{
		//filterMode: GL_NEAREST, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, etc
		//wrapMode; GL_REPEAT, GL_CLAMP_TO_BORDER, etc

		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		stbi_set_flip_vertically_on_load(true);

		//unsigned int texture; m_id;
		//glGenTextures(1, &texture);
		//glBindTexture(GL_TEXTURE_2D, texture);

		int width, height, nrChannels;

		unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}

		//unsigned int loadTexture2D(const char* filePath, int filterMode, int wrapMode);

		stbi_image_free(data);
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_id);
	}

	void Texture2D::Bind(unsigned int slot)
	{
		glActiveTexture(slot);
		glBindTexture(GL_TEXTURE_2D, m_id);
	}
}
