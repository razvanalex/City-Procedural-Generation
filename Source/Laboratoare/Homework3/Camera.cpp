#include "Camera.h"

Camera::Camera()
{
	position = glm::vec3(0, 2, 5);
	forward = glm::vec3(0, 0, -1);
	up = glm::vec3(0, 1, 0);
	right = glm::vec3(1, 0, 0);
	distanceToTarget = 2;
}

Camera::Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
{
	distanceToTarget = 1;
	Set(position, center, up);
}

Camera::~Camera()
{ }

// Update camera
void Camera::Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
{
	this->position = position;
	forward = glm::normalize(center - position);
	right = glm::cross(forward, up);
	this->up = glm::cross(right, forward);
}
void Camera::MoveForward(float distance)
{
	glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
}

void Camera::TranslateForward(float distance)
{
	position += forward * distance;
}

void Camera::TranslateRight(float distance)
{
	glm::vec3 r = glm::vec3(right.x, 0, right.z);
	position += right * distance;
}

void Camera::TranslateUpward(float distance)
{
	position += up * distance;
}

void Camera::RotateFirstPerson_OX(float angle)
{
	glm::vec4 newDir = glm::rotate(glm::mat4(1), angle, right)  * glm::vec4(forward, 1);
	forward = glm::normalize(glm::vec3(newDir));
	up = glm::normalize(glm::cross(right, forward));
}

void Camera::RotateFirstPerson_OY(float angle)
{
	glm::vec3 u = glm::vec3(0, glm::abs(up.y), 0);
	glm::vec4 newDir = glm::rotate(glm::mat4(1), angle, u)  * glm::vec4(forward, 1);
	forward = glm::normalize(glm::vec3(newDir));

	newDir = glm::rotate(glm::mat4(1), angle, u)  * glm::vec4(right, 1);
	right = glm::normalize(glm::vec3(newDir));

	up = glm::normalize(glm::cross(right, forward));
}

void Camera::RotateFirstPerson_OZ(float angle)
{
	glm::vec4 newDir = glm::rotate(glm::mat4(1), angle, forward)  * glm::vec4(forward, 1);
	forward = glm::normalize(glm::vec3(newDir));

	newDir = glm::rotate(glm::mat4(1), angle, forward)  * glm::vec4(right, 1);
	right = glm::normalize(glm::vec3(newDir));

	up = glm::normalize(glm::cross(right, forward));
}

void Camera::RotateThirdPerson_OX(float angle)
{
	TranslateForward(distanceToTarget);
	RotateFirstPerson_OX(angle);
	TranslateForward(-distanceToTarget);
}

void Camera::RotateThirdPerson_OY(float angle)
{
	TranslateForward(distanceToTarget);
	RotateFirstPerson_OY(angle);
	TranslateForward(-distanceToTarget);
}

void Camera::RotateThirdPerson_OZ(float angle)
{
	TranslateForward(distanceToTarget);
	RotateFirstPerson_OZ(angle);
	TranslateForward(-distanceToTarget);
}

glm::mat4 Camera::GetViewMatrix()
{
	// Returns the View Matrix
	return glm::lookAt(position, position + forward, up);
}

glm::vec3 Camera::GetTargetPosition()
{
	return position + forward * distanceToTarget;
}

glm::vec3 Camera::GetPosition()
{
	return this->position;
}
