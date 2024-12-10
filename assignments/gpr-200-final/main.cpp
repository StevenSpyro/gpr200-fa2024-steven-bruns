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

#include <vector>

#define _USE_MATH_DEFINES
#include <cmath>

using namespace myLibrary;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const unsigned int SHADOW_WIDTH = 1024;  // Width of the shadow map
const unsigned int SHADOW_HEIGHT = 1024; // Height of the shadow map

const int TERRAIN_FLOOR = 0.0f;
const int MAX_TREES = 50;

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

const int GRASS_OBJECT_NUM = 90000;
glm::vec3 windDir(2.0f, 0.0f, 0.0f);
float windSpeed = 1.0f;

bool blinn = false;

// Set Sun Speed
float sunSpeed = 0.25;

const double PI = 3.14159265358979323846;

void generateSphereVertices(float radius, int sectors, int stacks, std::vector<float>& sphereVertices, std::vector<unsigned int>& sphereIndices) {
	for (int i = 0; i <= stacks; ++i) {
		float stackAngle = glm::pi<float>() / 2 - i * glm::pi<float>() / stacks;  // [0, PI]
		float xy = radius * cos(stackAngle);  // Radius at current stack level
		float z = radius * sin(stackAngle);  // Z position

		for (int j = 0; j <= sectors; ++j) {
			float sectorAngle = j * 2 * glm::pi<float>() / sectors;  // [0, 2 * PI]
			float x = xy * cos(sectorAngle);  // X position
			float y = xy * sin(sectorAngle);  // Y position

			float u = (float)j / sectors;
			float v = (float)i / stacks;

			sphereVertices.push_back(x);
			sphereVertices.push_back(y);
			sphereVertices.push_back(z);
			sphereVertices.push_back(u);
			sphereVertices.push_back(v);

			// Generate indices for the triangles
			if (i < stacks && j < sectors) {
				unsigned int first = (i * (sectors + 1)) + j;
				unsigned int second = first + sectors + 1;

				sphereIndices.push_back(first);
				sphereIndices.push_back(second);
				sphereIndices.push_back(first + 1);

				sphereIndices.push_back(second);
				sphereIndices.push_back(second + 1);
				sphereIndices.push_back(first + 1);
			}
		}
	}
}

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Final Project", NULL, NULL);
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
	Shader grassShader("assets/grassVertexShader.vs", "assets/grassFragmentShader.fs");
	Shader skysphereShader("assets/skybox.vs", "assets/skybox.fs");

	//Instancing Setup | Brandon Cherry
	glm::vec2* translations = new glm::vec2[GRASS_OBJECT_NUM]; //Allocated on the Heap
	int index = 0;
	float offset = 0.5f;
	for (int y = -300; y < 300; y += 2)
	{
		for (int x = -300; x < 300; x += 2)
		{
			glm::vec2 translation;
			translation.x = (float)x / 7.0f + offset;
			translation.y = (float)y / 7.0f + offset;
			translations[index++] = translation;
		}
	}


	float treeVerts[] = {
		1.0f
	};

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

	float grassVertices[] = { //Brandon Cherry

		// Plane 1 (aligned with the XZ plane)
		-0.5f,  0.5f,  0.0f,   0.0f, 1.0f,  // Top-left
		-0.5f, -0.5f,  0.0f,   0.0f, 0.0f,  // Bottom-left
		 0.5f, -0.5f,  0.0f,   1.0f, 0.0f,  // Bottom-right

		-0.5f,  0.5f,  0.0f,   0.0f, 1.0f,  // Top-left
		 0.5f, -0.5f,  0.0f,   1.0f, 0.0f,  // Bottom-right
		 0.5f,  0.5f,  0.0f,   1.0f, 1.0f,  // Top-right

		// Plane 2 (rotated 45 degrees around Y-axis)
		-0.35f,  0.5f, -0.35f, 0.0f, 1.0f,  // Top-left
		-0.35f, -0.5f, -0.35f, 0.0f, 0.0f,  // Bottom-left
		 0.35f, -0.5f,  0.35f, 1.0f, 0.0f,  // Bottom-right

		-0.35f,  0.5f, -0.35f, 0.0f, 1.0f,  // Top-left
		 0.35f, -0.5f,  0.35f, 1.0f, 0.0f,  // Bottom-right
		 0.35f,  0.5f,  0.35f, 1.0f, 1.0f,  // Top-right



		// Plane 3 (rotated -45 degrees around Y-axis)
		-0.35f,  0.5f,  0.35f, 0.0f, 1.0f,  // Top-left
		-0.35f, -0.5f,  0.35f, 0.0f, 0.0f,  // Bottom-left
		 0.35f, -0.5f, -0.35f, 1.0f, 0.0f,  // Bottom-right

		-0.35f,  0.5f,  0.35f, 0.0f, 1.0f,  // Top-left
		 0.35f, -0.5f, -0.35f, 1.0f, 0.0f,  // Bottom-right
		 0.35f,  0.5f, -0.35f, 1.0f, 1.0f   // Top-right

	};

	unsigned int indices[] =
	{
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};


	// Area used to make the tree |Nick.M
	// Procedural geometry made with the help of Eric Winebrenner's example.
	struct treeVertex
	{
		glm::vec3 pos = glm::vec3(0);
		glm::vec3 normal = glm::vec3(0);
		glm::vec2 uv = glm::vec2(0);

		treeVertex() {}

		treeVertex(glm::vec3& pos, glm::vec3& normal, glm::vec2& uv) : pos(pos), normal(normal), uv(uv) {}
	};

	std::vector<treeVertex> treeVertices;
	std::vector<unsigned int> treeIndices;

	// variables for verts and Inds.
	int SubDivisions = 20; 
	float cRad = 2.0f;	 
	float thetaParts = 2 * PI / SubDivisions;
	float phiParts = PI / SubDivisions;

	// for the vertices
	for (int row = 0; row <= SubDivisions; row++)
	{
		float phi = row * phiParts;
		for (int col = 0; col <= SubDivisions; col++) 
		{
			float theta = col * thetaParts;

			// uv part
			glm::vec2 uv;
			uv.x = 1.0 - ((float)col / SubDivisions);
			uv.y = 1.0 - ((float)row / SubDivisions);

			// position part
			glm::vec3 pos;
			pos.x = cRad * cosf(theta) * sinf(phi);
			pos.y = cRad * cosf(phi);
			pos.z = cRad * sinf(theta) * sinf(phi);

			// normal part
			glm::vec3 normal = glm::normalize(pos);

			// psuh back each of these
			treeVertex tV;
			tV.pos = pos;
			tV.normal = normal;
			tV.uv = uv;

			treeVertices.push_back(tV);
		}
	}

	// for the indicies
	for (int r = 0; r < SubDivisions; r++)
	{
		for (int c = 0; c < SubDivisions; c++)
		{
			unsigned int bottomLeft = r * (SubDivisions + 1) + c;
			unsigned int bottomRight = bottomLeft + 1;
			unsigned int topLeft = bottomLeft + SubDivisions + 1;
			unsigned int topRight = topLeft + 1;

			treeIndices.push_back(bottomLeft);
			treeIndices.push_back(bottomRight);
			treeIndices.push_back(topRight);

			treeIndices.push_back(topRight);
			treeIndices.push_back(topLeft);
			treeIndices.push_back(bottomLeft);
		}

	}

	// This section will be for transfromations | Nick.M
	// This is for the base Terrain 
	glm::vec3 terrainPos;
	terrainPos.x = 0.0f;
	terrainPos.y = -1.0f;
	terrainPos.z = 0.0f;

	float terrainAngle;
	terrainAngle = 0.0f;

	glm::vec3 terrainRotate;
	terrainRotate.x = 0;
	terrainRotate.y = 0;
	terrainRotate.z = 0;

	glm::vec3 terrainScale;
	terrainScale.x = 200.0f;
	terrainScale.y = 1.0f;
	terrainScale.z = 200.0f;

	// This is for the tree base.
	float treeSpacing = terrainScale.x / MAX_TREES;

	float treesAngle[MAX_TREES];
	for (int i = 0; i < MAX_TREES; i++)
	{
		treesAngle[i] = 0.0f;
	}

	glm::vec3 treesRotate[MAX_TREES];
	for (int i = 0; i < MAX_TREES; i++)
	{
		treesRotate[i].x = 45.0f;
		treesRotate[i].y = 0.0f;
		treesRotate[i].z = 0.0f;
	}

	glm::vec3 treesScale[MAX_TREES];
	for (int i = 0; i < MAX_TREES; i++)
	{
		treesScale[i].x = 0.5f;
		treesScale[i].y = ew::RandomRange(5.0f, 15.0f);
		treesScale[i].z = 0.5f;
	}

	glm::vec3 treesPos[MAX_TREES];
	for (int i = 0; i < MAX_TREES; i++) 
	{
		treesPos[i].x = 100.0f - (treeSpacing * i);  
		treesPos[i].y = TERRAIN_FLOOR + (treesScale[i].y / 2);
		treesPos[i].z = ew::RandomRange(-100.0, -1.0f);
	}

	// This is for the tree tops 
	glm::vec3 treeTopPos[MAX_TREES];
	for (int i = 0; i < MAX_TREES; i++)
	{
		treeTopPos[i].x = treesPos[i].x;
		treeTopPos[i].y = treesScale[i].y;
		treeTopPos[i].z = treesPos[i].z;
	}

	float treeTopAngle[MAX_TREES];
	for (int i = 0; i < MAX_TREES; i++)
	{
		treeTopAngle[i] = 0.0f;
	}

	glm::vec3 treeTopRotate[MAX_TREES];
	for (int i = 0; i < MAX_TREES; i++)
	{
		treeTopRotate[i].x = 45.0f;
		treeTopRotate[i].y = 0.0f;
		treeTopRotate[i].z = 0.0f;
	}

	glm::vec3 treeTopScale[MAX_TREES];
	for (int i = 0; i < MAX_TREES; i++)
	{
		treeTopScale[i].x = 1.0f;
		treeTopScale[i].y = 1.0f;
		treeTopScale[i].z = 1.0f;
	}
	// end of transformation section, add more if we need to.

	//Geometry for the Sky Sphere
	const int X_SEGMENTS = 64;
	const int Y_SEGMENTS = 64;
	const float RADIUS = 1.0f;
 
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
	Texture2D texture0("assets/Brown.png", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_RGBA);

	texture0.Bind(GL_TEXTURE0);

	// Set the textures to ints
	lightingShader.setInt("texture0", 0);

	//Grass VAO and VBO | Brandon Cherry
	unsigned int grassVAO, grassVBO;
	glGenVertexArrays(1, &grassVAO);
	glGenBuffers(1, &grassVBO);
	glBindVertexArray(grassVAO);
	glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grassVertices), grassVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));


	//Instances | Brandon Cherry
	unsigned int instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * GRASS_OBJECT_NUM, &translations[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	glVertexAttribDivisor(2, 1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Sphere VAO and VBO | Steven Bruns

	std::vector<float> sphereVertices;
	std::vector<unsigned int> sphereIndices;

	generateSphereVertices(1000.0f, 50, 50, sphereVertices, sphereIndices);

	GLuint sphereVAO, sphereVBO, sphereEBO;
	glGenVertexArrays(1, &sphereVAO);
	glGenBuffers(1, &sphereVBO);
	glGenBuffers(1, &sphereEBO);

	glBindVertexArray(sphereVAO);

	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
	glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), &sphereVertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(unsigned int), &sphereIndices[0], GL_STATIC_DRAW);

	// Setup vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // Position
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // UVs
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Shaders FBO | Steven Bruns
	//Cancelled due to lack of time, and it was super complicated

	// This will be for tree attributes | Nick.M
	unsigned int treeVAO, treeVBO, TreeEBO;

	// The VAO
	glGenVertexArrays(1, &treeVAO);
	glBindVertexArray(treeVAO);

	// Next VBO
	glGenBuffers(1, &treeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, treeVBO);

	// Next EBO
	glGenBuffers(1, &TreeEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TreeEBO);

	// vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(treeVertex), (void*)offsetof(treeVertex, pos)); // Position
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(treeVertex), (void*)offsetof(treeVertex, normal)); // Normals
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(treeVertex), (void*)offsetof(treeVertex, uv)); // UVs
	glEnableVertexAttribArray(2);

	// Buffer binding part
	glBufferData(GL_ARRAY_BUFFER, treeVertices.size() * sizeof(treeVertex), &treeVertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, treeIndices.size() * sizeof(unsigned int), &treeIndices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//Grass Texture and shader | Brandon Cherry
	Texture2D grassTexture("assets/GrassTexture.png", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE , GL_RGBA);
	grassTexture.Bind(GL_TEXTURE1);
	grassShader.use();
	grassShader.setInt("grassTexture", 0);	

	//Sky Sphere Texture | Steven Bruns from when I wanted to use a texture

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

	float ambientK = 0.5f;
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
		float adjustedTime = time * sunSpeed;
		//skysphereShader.setFloat("_Time", adjustedTime);
		glm::vec3 sunDir = glm::normalize(glm::vec3(cos(adjustedTime), sin(adjustedTime), 0.0f));

		lightingShader.use();
		lightingShader.setVec3("sunDir", sunDir);

		skysphereShader.use();
		skysphereShader.setVec3("sunDir", sunDir);


		// Clear framebuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glClear(GL_DEPTH_BUFFER_BIT);					 // Clear

		glUniform1f(timeLoc, time);
		glUniform1f(ambientLoc, ambientK);
		glUniform1f(diffLoc, diffuseK);
		glUniform1f(specularLoc, specularK);
		glUniform1f(shininessLoc, shininess);

		glm::vec3 lightDir = glm::vec3(cos(time), -0.5f, sin(time)); // Simulate sun movement
		lightingShader.use();
		lightingShader.setVec3("lightDir", lightDir);

		//lightingShader.use();
		lightingShader.setVec3("lightPos", lightPos);
		lightingShader.setVec3("lightColor", lightColor);
		lightingShader.setVec3("viewPos", cam.Position);
		lightingShader.setFloat("ambientK", ambientK);
		lightingShader.setFloat("diffuseK", diffuseK);
		lightingShader.setFloat("specularK", specularK);
		lightingShader.setFloat("shininess", shininess);

		glUniform1f(timeLoc, time);

		// Set Projection
		glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);
		glm::mat4 view = cam.GetViewMatrix();

		// Draw SkySphere
		glDisable(GL_DEPTH_TEST); //DISABLE
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_LEQUAL);

		skysphereShader.use();
		skysphereShader.setMat4("_ViewProjection", projection * view);
		skysphereShader.setFloat("_Time", time);
		skysphereShader.setVec3("sunCol", glm::vec3(1.0f, 0.8f, 0.5f));
		glm::mat4 sphereModel = glm::scale(glm::mat4(1.0f), glm::vec3(1000.0f));
		skysphereShader.setMat4("_Model", sphereModel);
		skysphereShader.setFloat("sunSpeed", sunSpeed);

		glBindVertexArray(sphereVAO);
		glDrawArrays(GL_TRIANGLES, 0, sphereVertices.size() / 5);
		glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);
	
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS); // Default depth function
		glEnable(GL_DEPTH_TEST); // Re-enable depth testing

		// Bind 
		texture0.Bind(GL_TEXTURE0);
		texture0.Texture2D::Bind(GL_TEXTURE0);

		//Activate Shader to set uniforms/draw objects
		lightingShader.use();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		glBindVertexArray(VAO);

		// This is for the terrain | Nick.M
		// Credit to Steven for the original code, thank you.
		for (unsigned int i = 0; i < 21; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::scale(model, terrainScale);
			model = glm::translate(model, terrainPos);
			//float angle = 20.0f * i;
			//model = glm::rotate(model, rotateTime * glm::radians(rotateterrainAngle[i]), terrainRotate[i]);
			lightingShader.setMat4("model", model);

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		// this part is to draw the objects on the terrain | Nick.M
		for (unsigned int i = 0; i < MAX_TREES; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			
			model = glm::translate(model, treesPos[i]);
			// skipping rotation, not needed otherwise here
			model = glm::scale(model, treesScale[i]); 
			lightingShader.setMat4("model", model);

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		

		// for the tree tops to appear | Nick.M
		for (unsigned int i = 0; i < MAX_TREES; i++) 
		{
			glm::mat4 treeModel = glm::mat4(1.0f);
			
			treeModel = glm::translate(treeModel, treeTopPos[i]);
			// skipping rotation, not needed otherwise here
			treeModel = glm::scale(treeModel, treeTopScale[i]);
			lightingShader.setMat4("model", treeModel);

			int vertsAmount = treeVertices.size();
			int indsAmount = treeIndices.size();

			glBindVertexArray(treeVAO);
			glDrawElements(GL_TRIANGLES, indsAmount, GL_UNSIGNED_INT, NULL);
			// glDrawArrays(GL_TRIANGLES, 0, vertsAmount); // dont use this
		}
		

		//Drawing Grass using grassShader and grassVAO | Brandon Cherry
		grassShader.use();
		grassShader.setMat4("projection", projection);
		grassShader.setMat4("view", view);
		glBindVertexArray(grassVAO);
		grassTexture.Bind(GL_TEXTURE0);
		glm::mat4 model2 = glm::mat4(1.0f);
		grassShader.setMat4("model", model2);
		grassShader.setFloat("_time", time);

		grassShader.setFloat("windSpeed", windSpeed);
		grassShader.setVec3("windDirection", windDir);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 18, GRASS_OBJECT_NUM);
		
		glBindVertexArray(VAO);

		glBindVertexArray(0);


		// Start Drawing ImGUI
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		// Create a window called Settings.
		ImGui::Begin("Settings");
		ImGui::Text("Controls");
		//ImGui::DragFloat3("Light Position", &lightPos.x, 0.1f);
		ImGui::ColorEdit3("World Color", &lightColor.r);
		ImGui::SliderFloat("Ambient K", &ambientK, 0.0f, 1.0f);
		ImGui::SliderFloat("Diffuse K", &diffuseK, 0.0f, 1.0f);
		ImGui::SliderFloat("Specular K", &specularK, 0.0f, 1.0f);
		ImGui::SliderFloat("Shininess", &shininess, 2.0f, 1024.0f);
		ImGui::SliderFloat("Sun Speed", &sunSpeed, 0.01f, 1.0f);

		ImGui::DragFloat3("Wind Direction", &windDir.x, 0.1f);
		ImGui::SliderFloat("Wind Speed", &windSpeed, 0.0f, 2.5f);

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

	delete[] translations; //Deallocate translations on the heap
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
	float yoffset = -ypos + lastY;
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
