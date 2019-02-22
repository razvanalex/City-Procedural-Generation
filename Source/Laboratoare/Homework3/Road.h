#pragma once
#include "Object3D.h"

class Road : public Object3D {
public:
	Road(char *name, glm::vec3 position, glm::vec3 rotation,
		glm::vec3 scale, glm::vec3 color);
	~Road();

	void Road::Init() override;
};