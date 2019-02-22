#include "Object3D.h"

Object3D::Object3D(std::vector<VertexFormat> vertices,
	std::vector<unsigned short> indices, glm::mat4 modelMatrix,
	char *name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale,
	glm::vec3 color)
{
	this->vertices = vertices;
	this->indices = indices;
	this->modelMatix = modelMatix;
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
	this->color = color;
	this->name = name;
	this->mesh = NULL;

	UpdateModelMatrix();
}

Object3D::Object3D(char *name, glm::vec3 position, glm::vec3 rotation,
	glm::vec3 scale, glm::vec3 color)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
	this->color = color;
	this->name = name;

	Init();
	UpdateModelMatrix();
}
Object3D::~Object3D()
{
}

void Object3D::Init()
{
}

void Object3D::SetModelMatrix(glm::mat4 modelMatrix)
{
	this->modelMatix = modelMatix;
}

void Object3D::ApplyTransformation(glm::mat4 transformation)
{
	this->modelMatix = transformation * this->modelMatix;
}

glm::mat4 Object3D::GetModelMatrix()
{
	return this->modelMatix;
}

Mesh* Object3D::CreateMesh()
{
	unsigned int VAO = 0;
	// Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Create the VBO and bind it
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Send vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// Crete the IBO and bind it
	unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data

	// set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	// Unbind the VAO
	glBindVertexArray(0);

	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	Mesh* mesh = new Mesh(name);
	mesh->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	mesh->vertices = this->vertices;
	mesh->indices = this->indices;

	mesh->SetDrawMode(drawMode);

	return mesh;
}

void Object3D::RenderMesh(Shader *shader, Properties *props)
{
	if (!shader || !shader->GetProgramID())
		return;

	if (!mesh)
		mesh = CreateMesh();
	if (!mesh)
		return;

	glUseProgram(shader->program);

	props->ApplyAll();

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

char * Object3D::GetName()
{
	return this->name;
}


void Object3D::UpdateModelMatrix()
{
	this->modelMatix = Transform3D::Translate(position.x, position.y, position.z) *
		Transform3D::RotateOX(rotation.x) *
		Transform3D::RotateOY(rotation.y) *
		Transform3D::RotateOZ(rotation.z) *
		Transform3D::Scale(scale.x, scale.y, scale.z);
}

void Object3D::SetPosition(glm::vec3 position)
{
	this->position = position;
}

glm::vec3 Object3D::GetPosition()
{
	return this->position;
}

void Object3D::SetRotation(glm::vec3 rotation)
{
	this->rotation = rotation;
}

glm::vec3 Object3D::GetRotation()
{
	return this->rotation;
}

void Object3D::SetScale(glm::vec3 scale)
{
	this->scale = scale;
}

glm::vec3 Object3D::GetScale()
{
	return this->scale;
}

glm::vec3 Object3D::GetColor()
{
	return this->color;
}
