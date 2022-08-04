#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldup)
{
	Position = position;
	WorldUp = worldup;
	Forward = glm::normalize(target - position);
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Right,Forward));
}

Camera::Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup)
{
	Position = position;
	WorldUp = worldup;
	Pitch = pitch;
	Yaw = yaw;
	Forward.x = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
	Forward.y = sin(glm::radians(Pitch));
	Forward.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Right, Forward));
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position,Position+Forward,WorldUp);
}

void Camera::ProcessMouseMovement(float deltaX, float deltaY)
{
	Pitch -= deltaY*0.05f;
	Yaw += deltaX*0.05f;
	UpdateCameraVectors();


}

void Camera::UpdateCameraPos()
{
	Position += (Forward * speedZ + Right * speedX + Up * speedY)*4.0f ;
}

void Camera::UpdateCameraVectors()
{
	Forward.x = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
	Forward.y = sin(glm::radians(Pitch));
	Forward.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Right, Forward));
}
