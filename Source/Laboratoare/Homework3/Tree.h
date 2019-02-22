#pragma once
#include "Object3D.h"

class Tree : public Object3D {
public:
	Tree(char * name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale,
		glm::vec3 color) : Object3D(name, position, rotation, scale, color)
	{
		this->SetRotation(glm::vec3(-glm::half_pi<float>(), 0, 0));
		//this->SetScale(scale * glm::vec3(1));
		this->UpdateModelMatrix();
	}

	~Tree() {}

private:
	Mesh* CreateMesh() override
	{
		// Load meshes
		Mesh* mesh = new Mesh(this->name);
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "\/Vegetation\/Tree", "Tree.fbx");

		return mesh;
	}
};