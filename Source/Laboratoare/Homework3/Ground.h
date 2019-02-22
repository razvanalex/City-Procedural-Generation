#pragma once
#include "Object3D.h"

class Ground : public Object3D {
public:
	Ground(char *name, glm::vec3 position, glm::vec3 rotation,
		glm::vec3 scale, glm::vec3 color) : Object3D(name, position, rotation, scale, color)
	{
		this->UpdateModelMatrix();
		this->Init();
	}


	~Ground() {}

	void Ground::Init()
	{
		this->vertices = {
		   VertexFormat(glm::vec3(-1,  0,  1), glm::vec3(1), glm::vec3(0, 1, 0), glm::vec2(2 * scale.x, 2 * scale.z)),
		   VertexFormat(glm::vec3(1,  0,  1), glm::vec3(1), glm::vec3(0, 1, 0), glm::vec2(0, 2 * scale.z)),
		   VertexFormat(glm::vec3(1,  0, -1), glm::vec3(1), glm::vec3(0, 1, 0), glm::vec2(0, 0)),
		   VertexFormat(glm::vec3(-1,  0, -1), glm::vec3(1), glm::vec3(0, 1, 0), glm::vec2(2 * scale.x, 0))
		};

		this->indices = {
			0, 1, 2,
			0, 2, 3
		};

	}
};