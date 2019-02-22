#pragma once
#include <include/glm.h>
#include <include/math.h>

class Camera
{
private:
	float distanceToTarget = 2;

	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;

public: 
	Camera();
	Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up);
	~Camera();

	void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up);

	void MoveForward(float distance);
	void TranslateForward(float distance);
	void TranslateUpward(float distance);
	void TranslateRight(float distance);

	void RotateFirstPerson_OX(float angle);
	void RotateFirstPerson_OY(float angle);
	void RotateFirstPerson_OZ(float angle);

	void RotateThirdPerson_OX(float angle);
	void RotateThirdPerson_OY(float angle);
	void RotateThirdPerson_OZ(float angle);

	glm::mat4 GetViewMatrix();
	glm::vec3 GetTargetPosition();
	glm::vec3 GetPosition();
};