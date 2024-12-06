#include "texture2D.h"

using namespace std;

namespace myLibrary
{
	Texture2D::Texture2D() // Default constructor
	{
		glGenTextures(1, &m_id); 
		glBindTexture(GL_TEXTURE_2D, m_id); 
	}

	Texture2D::Texture2D(const char* filePath, int filterModeMin, int filterModeMag, int wrapModeS, int wrapModeT, int alpha)
	{
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapModeS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapModeT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterModeMin);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterModeMag);

		stbi_set_flip_vertically_on_load(true);

		int width, height, nrChannels;

		unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);

		// Get the image data
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, alpha, width, height, 0, alpha, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}

		stbi_image_free(data);
	}

	Texture2D::~Texture2D()
	{
	}

	void Texture2D::CreateDepthTexture(int width, int height)
	{
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);

		// Create a depth texture for shadow mapping
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

		// Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Set wrapping mode (no wrapping for depth map)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		// Set the border color to white (this is important for depth textures)
		GLfloat borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		// Generate the texture
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	// Bind the Texture
	void Texture2D::Bind(unsigned int slot)
	{
		glActiveTexture(slot);
		glBindTexture(GL_TEXTURE_2D, m_id);
	}

}
