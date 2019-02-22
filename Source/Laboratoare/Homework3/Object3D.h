#pragma once
#include <include/glm.h>
#include <vector>
#include <Core/Engine.h>

#include "Transform3D.h"
#include "Properties.h"

class Object3D {
public:
	Object3D(std::vector<VertexFormat> vertices,
		std::vector<unsigned short> indices, glm::mat4 modelMatrix,
		char *name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale,
		glm::vec3 color);
	Object3D(char *name, glm::vec3 position, glm::vec3 rotation,
		glm::vec3 scale, glm::vec3 color);
	~Object3D();

private:
	virtual void Init();

protected:
	char *name;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::vec3 color;
	glm::mat4 modelMatix;
	Mesh* mesh;

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;
	bool textureEnable = false;

	int drawMode = GL_TRIANGLES;

public: void UpdateModelMatrix();

public:
	void SetModelMatrix(glm::mat4 modelMatrix);
	void ApplyTransformation(glm::mat4 transformation);
	glm::mat4 GetModelMatrix();
	virtual Mesh* CreateMesh();
	char* GetName();

	void SetPosition(glm::vec3 position);
	glm::vec3 GetPosition();
	void SetRotation(glm::vec3 rotation);
	glm::vec3 GetRotation();
	void SetScale(glm::vec3 scale);
	glm::vec3 GetScale();
	glm::vec3 GetColor();

	void RenderMesh(Shader *shader, Properties *props);
};