#ifndef CAMERA_H
#define CAMERA_H

#include"../ew/external/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

//Camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

//Camera class. Calculates angles, vectors, and Matrices. Processes Input.

class Camera
{
public:
	//Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	//Euler Angles
	float Yaw;
	float Pitch;

	//Camera Options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	//Vector Constructor
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

	//Scalar Values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	//Return view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix();

	//Input System
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);

	//Mouse Input
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

	//Mouse Scroll
	void ProcessMouseScroll(float yoffset);

private:
	//Calculates the front vector from the Cameras updated Euler Angles

	void updateCameraVectors();

};

#endif