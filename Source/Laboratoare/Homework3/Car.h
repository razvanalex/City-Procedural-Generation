#pragma once
#include "Object3D.h"

class Car : public Object3D {
public:
	Car(char * name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale,
		glm::vec3 color) : Object3D(name, position, rotation, scale, color)
	{
		this->SetRotation(this->GetRotation() + glm::vec3(-glm::half_pi<float>(), 0, 0));
		this->SetScale(scale * glm::vec3(0.4));
		this->SetPosition(this->GetPosition() + glm::vec3(0, 0.1, 0));
		this->UpdateModelMatrix();
	}

	~Car() {}

private:
	Mesh* CreateMesh() override
	{
		// Load meshes
		Mesh* mesh = new Mesh(this->name);
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "\/Cars", "lexus.fbx");

		return mesh;
	}
};