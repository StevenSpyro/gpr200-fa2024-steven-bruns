#include "camera.h"


// Vector Constructor
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();

	Front = glm::vec3(0.0f, 0.0f, -1.0f);
	MovementSpeed = SPEED;
	MouseSensitivity = SENSITIVITY;
	Zoom = ZOOM;
}

// Scalar Values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
{
	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();

	Front = glm::vec3(0.0f, 0.0f, -1.0f);
	MovementSpeed = SPEED;
	MouseSensitivity = SENSITIVITY;
	Zoom = ZOOM;
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

// Input System
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;

	// Switch for each input
	switch (direction)
	{
	case FORWARD:
		Position += Front * velocity;
		break;
	case BACKWARD:
		Position -= Front * velocity;
		break;
	case LEFT:
		Position -= Right * velocity;
		break;
	case RIGHT:
		Position += Right * velocity;
		break;
	case UP:
		Position += Up * velocity;
		break;
	case DOWN:
		Position -= Up * velocity;
		break;
	case WALK:
		MovementSpeed = SPEED;
		break;
	case SPRINT:
		MovementSpeed = SPEED * 2.0;
		break;
	}
}

// Mouse Input
void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	// Avoid flipping
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
		{
			Pitch = 89.0f;
		}
		if (Pitch < -89.0f)
		{
			Pitch = -89.0f;
		}
	}

	updateCameraVectors();

}
// Mouse Scroll
void Camera::ProcessMouseScroll(float yoffset)
{
	Zoom -= (float)yoffset;

	if (Zoom < 1.0f)
	{
		Zoom = 1.0f;
	}
	if (Zoom > 120.0f)
	{
		Zoom = 120.0f;
	}
}
void Camera::updateCameraVectors()
{
		// Calcs the new front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// Recalc the right and up vec
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
}