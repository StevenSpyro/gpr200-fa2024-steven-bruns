#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <MyLibrary/shader.h>
#include <MyLibrary/texture2D.h>
#include <MyLibrary/camera.h>

#define STB_IMAGE_IMPLEMENTATION
#include "ew/external/stb_image.h"

using namespace myLibrary;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));

void processInput(GLFWwindow* window);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool isOrthograph = false;
bool firstMouse = true;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;


int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Assignment 4", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST); //Need Now
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scroll_callback);

	Shader charShader("assets/vertexShader.vs", "assets/fragmentShader.fs");
	Shader bgShader("assets/vertexShaderBG.vs", "assets/fragmentShaderBG.fs");

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	unsigned int indices[] =
	{
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	//Set Random Positions
	glm::vec3 posRand[20];
	for (int i = 0; i < 20; i++)
	{
		posRand[i].x = ew::RandomRange(-5.0, 5.0f);
		posRand[i].y = ew::RandomRange(-5.0, 5.0f);
		posRand[i].z = ew::RandomRange(-10.0, -5.0f);
	}

	//Set Random Angles
	float rotateAngleRand[20];
	for (int i = 0; i < 20; i++)
	{
		rotateAngleRand[i] = ew::RandomRange(-179, 179);
	}

	//Set Random Axis
	glm::vec3 rotateAxisRand[20];
	for (int i = 0; i < 20; i++)
	{
		rotateAxisRand[i].x = ew::RandomRange(-10.0, 10.0f);
		rotateAxisRand[i].y = ew::RandomRange(-10.0, 10.0f);
		rotateAxisRand[i].z = ew::RandomRange(-10.0, 0.0f);
	}

	//Set Random Scale
	glm::vec3 scaleRand[20];
	for(int i = 0; i < 20; i++)
	{
		scaleRand[i].x = ew::RandomRange(1.0f, 20.0f);
		scaleRand[i].y = ew::RandomRange(1.0f, 20.0f);
		scaleRand[i].z = ew::RandomRange(1.0f, 20.0f);
	}

	//Initialization goes here!
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// Copy Vertices

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Set the vertex attributes pointers

	// Position (XYZ)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Texture coords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Grab the textures
	Texture2D texture0("assets/Water.png", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_RGBA);
	Texture2D texture1("assets/Link.png", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_RGBA);

	bgShader.Shader::use();

	// Set the textures to ints
	bgShader.setInt("texture1", 0);
	bgShader.setInt("texture2", 1);

	float rotateTime = 0;

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Set Frame and keep track for Rotation sake.
		float currentFrame = (glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		rotateTime += deltaTime;

		processInput(window);

		// Set Time
		float time = (float)glfwGetTime();
		int timeLoc = glGetUniformLocation(bgShader.ID, "uTime");
		glUniform1f(timeLoc, time);

		// Clear framebuffer
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Bind 
		texture0.Texture2D::Bind(GL_TEXTURE0); 
		texture1.Texture2D::Bind(GL_TEXTURE1); 

		bgShader.Shader::use();

		// Set Projection
		glm::mat4 projection;
		if (isOrthograph)
		{
			float aspectRatio = 800.0f / 600.0f;
			projection = glm::ortho(-10.0f, 10.0f, -10.0f / aspectRatio, 0.5f / aspectRatio, 0.1f, 100.0f);
		}
		else
		{
			projection = glm::perspective(glm::radians(cam.Zoom), 800.0f / 600.0f, 0.1f, 1000.0f);
		}
		bgShader.setMat4("projection", projection);

		//Original integration before Ortho
		/*
		glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), 800.0f / 600.0f, 0.1f, 1000.0f);
		bgShader.setMat4("projection", projection);
		*/

		// Set View
		glm::mat4 view = cam.GetViewMatrix();
		bgShader.setMat4("view", view);


		// Draw
		glBindVertexArray(VAO);

		for (unsigned int i = 0; i < 20; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::scale(model, scaleRand[i]);
			model = glm::translate(model, posRand[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, rotateTime * glm::radians(rotateAngleRand[i]), rotateAxisRand[i]);
			bgShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwPollEvents();

		glfwSwapBuffers(window);
	}

	// Clear the heap because it was annoying seeing all the warnings.
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();

	printf("Shutting down...");
}

// Allow user to Keyboard Input
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
	{
		isOrthograph = !isOrthograph;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cam.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cam.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cam.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cam.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		cam.ProcessKeyboard(WALK, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		cam.ProcessKeyboard(SPRINT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		cam.ProcessKeyboard(DOWN, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		cam.ProcessKeyboard(UP, deltaTime);
	}
}

// Allow user to use Mouse
void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;

	cam.ProcessMouseMovement(xoffset, yoffset);
}

// Allow user to Scroll
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	cam.ProcessMouseScroll(yoffset);
}