#pragma once

#include "Object3D.h"

class Cylinder : public Object3D {
public:
	Cylinder(char * name, glm::vec3 position, glm::vec3 rotation,
		glm::vec3 scale, glm::vec3 color, int sharpness, float radius1,
		float radius2, bool textureEnable, glm::vec2 textureScale);
	~Cylinder();

	bool textureEnable;
private:
	float radius1;
	float radius2;
	int sharpness;
	bool skipEdge = false;
	glm::vec2 textureScale;

	void Init() override;
};