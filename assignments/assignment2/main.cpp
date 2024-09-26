#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <MyLibrary/shader.h>
#include <MyLibrary/Texture2D.h>

#define STB_IMAGE_IMPLEMENTATION
#include "ew/external/stb_image.h"

using namespace myLibrary;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

Shader ourShader("vertexShader.vs", "fragmentShader.fr");

float vertices[] =
{    // positions         // colors           // texture coords
	 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top right
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom right
	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // bottom left
	-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // top left
};


unsigned int indices[] = 
{ 
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};


int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	//Initialization goes here!
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	unsigned int EBO;
	glGenBuffers(1, &EBO);

	// 0. copy our vertices array in a buffer for OpenGL to use

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 1. then set the vertex attributes pointers

	//Position (XYZ)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//COLOR(RGBA)
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);

	Texture2D texture("assets/Link.png",0, 0);
	Texture2D texture1("assets/Water.png", 0, 0);


	//Render loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		float time = (float)glfwGetTime();

		//Clear framebuffer
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1.getID());

		glBindVertexArray(VAO); 

		//Draw Call
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		//glActiveTexture(GL_TEXTURE0); 

		//glBindTexture(GL_TEXTURE_2D, texture);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		//Drawing happens here!

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
	}

	printf("Shutting down...");
}
