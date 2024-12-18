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

//IMGUI
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

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
float lastX = (float) SCREEN_WIDTH / 2.0;
float lastY = (float) SCREEN_HEIGHT / 2.0;

float ambientK = 0.1f;
float diffuseK = 1.0f;
float specularK = 1.0f;
float shininess = 32.0f;

bool blinn = false;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Assignment 5", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwMakeContextCurrent(window);
	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST); //Need Now
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader lightingShader("assets/vertexShader.vs", "assets/fragmentShader.fs");
	Shader lightCubeShader("assets/vertexShaderBG.vs", "assets/fragmentShaderBG.fs");

	float vertices[] = {
		//  X      Y      Z      U     V     NX     NY     NZ
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
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
	float angleRand[20];
	for (int i = 0; i < 20; i++)
	{
		angleRand[i] = ew::RandomRange(-179, 179);
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

	// Initialization Goes Here
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Grab the textures
	Texture2D texture0("assets/Wood.png", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_RGBA);

	texture0.Bind(GL_TEXTURE0);

	// Set the textures to ints
	lightingShader.setInt("texture0", 0);

	float rotateTime = 0;

	//Lighting
	glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

	//Loc
	int timeLoc = glGetUniformLocation(lightingShader.ID, "uTime");
	int ambientLoc = glGetUniformLocation(lightingShader.ID, "ambientK");
	int diffLoc = glGetUniformLocation(lightingShader.ID, "diffuseK");
	int specularLoc = glGetUniformLocation(lightingShader.ID, "specularK");
	int shininessLoc = glGetUniformLocation(lightingShader.ID, "shininess");
	int lightColorLoc = glGetUniformLocation(lightingShader.ID, "lightColor");

	float ambientK = 0.1f;
	float diffuseK = 1.0f;
	float specularK = 0.5f;
	float shininess = 256.0f;

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

		// Clear framebuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUniform1f(timeLoc, time);
		glUniform1f(ambientLoc, ambientK);
		glUniform1f(diffLoc, diffuseK);
		glUniform1f(specularLoc, specularK);
		glUniform1f(shininessLoc, shininess);

		//Activate Shader to set uniforms/draw objects

		lightingShader.use();
		lightingShader.setVec3("lightPos", lightPos);
		lightingShader.setVec3("lightColor", lightColor);
		lightingShader.setVec3("viewPos", cam.Position);

		lightingShader.setFloat("ambientK", ambientK);
		lightingShader.setFloat("diffuseK", diffuseK);
		lightingShader.setFloat("specularK", specularK);
		lightingShader.setFloat("shininess", shininess);

		texture0.Bind(GL_TEXTURE0);

		glUniform1f(timeLoc, time);

		// Bind 
		texture0.Texture2D::Bind(GL_TEXTURE0); 

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

		//Original integration before Ortho
		/*
		glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), 800.0f / 600.0f, 0.1f, 1000.0f);
		bgShader.setMat4("projection", projection);
		*/

		// Set View
		glm::mat4 view = cam.GetViewMatrix();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);


		// Draw
		glBindVertexArray(VAO);

		for (unsigned int i = 0; i < 20; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::scale(model, scaleRand[i]);
			model = glm::translate(model, posRand[i]);
			//float angle = 20.0f * i;
			//model = glm::rotate(model, rotateTime * glm::radians(rotateAngleRand[i]), rotateAxisRand[i]);
			lightingShader.setMat4("model", model);

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);

		}

		// also draw the lamp object
		lightCubeShader.use();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setFloat("uTime", time);
		lightCubeShader.setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.8f)); // a smaller cube


		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		lightCubeShader.setMat4("model", model);
		lightCubeShader.setVec3("lightColor", lightColor);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);


		// Start Drawing ImGUI
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		// Create a window called Settings.
		//ImGui::Text("Add Controls Here!");
		ImGui::Begin("Settings");
		ImGui::Text("Controls");
		ImGui::DragFloat3("Light Position", &lightPos.x, 0.1f);
		ImGui::ColorEdit3("Light Color", &lightColor.r);
		ImGui::SliderFloat("Ambient K", &ambientK, 0.0f, 1.0f);
		ImGui::SliderFloat("Diffuse K", &diffuseK, 0.0f, 1.0f);
		ImGui::SliderFloat("Specular K", &specularK, 0.0f, 1.0f);
		ImGui::SliderFloat("Shininess", &shininess, 2.0f, 1024.0f);

		ImGui::End();

		//Actually render IMGUI elements using OpenGL
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwPollEvents();

		glfwSwapBuffers(window);
	}

	// Clear the heap because it was annoying seeing all the warnings.
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
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
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) //Lock Mouse
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE) //Unlock Mouse
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	//if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
	//{
	//	isOrthograph = !isOrthograph;
	//}
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

	if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
	{
		cam.ProcessMouseMovement(xoffset, yoffset);
	}
}

// Allow user to Scroll
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	cam.ProcessMouseScroll(yoffset);
}