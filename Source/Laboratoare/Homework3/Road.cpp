#include "Road.h"

Road::Road(char * name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, 
	glm::vec3 color) : Object3D(name, position, rotation, scale, color)
{
	this->UpdateModelMatrix();
	this->Init();
}

void Road::Init() 
{
	this->vertices = {
		// FORWARD
	   VertexFormat(glm::vec3(-1, -1, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec2(1, 1)),
	   VertexFormat(glm::vec3( 1, -1, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec2(0, 1)),
	   VertexFormat(glm::vec3( 1,  1, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec2(0, 0)),
	   VertexFormat(glm::vec3(-1,  1, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec2(1, 0)),

	   // RIGHT
	   VertexFormat(glm::vec3(1, -1,  1), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), glm::vec2(1, 1)),
	   VertexFormat(glm::vec3(1, -1, -1), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), glm::vec2(0, 1)),
	   VertexFormat(glm::vec3(1,  1, -1), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), glm::vec2(0, 0)),
	   VertexFormat(glm::vec3(1,  1,  1), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), glm::vec2(1, 0)),

	   // TOP
	   VertexFormat(glm::vec3(-1,  1,  1), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec2(1, scale.z)),
	   VertexFormat(glm::vec3(1,  1,  1), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec2(0, scale.z)),
	   VertexFormat(glm::vec3(1,  1, -1), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec2(0, 0)),
	   VertexFormat(glm::vec3(-1,  1, -1), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec2(1, 0)),

	   // LEFT
	   VertexFormat(glm::vec3(-1, -1,  1), glm::vec3(1, 1, 0), glm::vec3(-1, 0, 0), glm::vec2(1, 1)),
	   VertexFormat(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 0), glm::vec3(-1, 0, 0), glm::vec2(0, 1)),
	   VertexFormat(glm::vec3(-1,  1, -1), glm::vec3(1, 1, 0), glm::vec3(-1, 0, 0), glm::vec2(0, 0)),
	   VertexFormat(glm::vec3(-1,  1,  1), glm::vec3(1, 1, 0), glm::vec3(-1, 0, 0), glm::vec2(1, 0)),

	   // BACK
	   VertexFormat(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1), glm::vec3(0, 0, -1), glm::vec2(1, 1)),
	   VertexFormat(glm::vec3(1, -1, -1), glm::vec3(1, 1, 1), glm::vec3(0, 0, -1), glm::vec2(0, 1)),
	   VertexFormat(glm::vec3(1,  1, -1), glm::vec3(1, 1, 1), glm::vec3(0, 0, -1), glm::vec2(0, 0)),
	   VertexFormat(glm::vec3(-1,  1, -1), glm::vec3(1, 1, 1), glm::vec3(0, 0, -1), glm::vec2(1, 0)),

	   // BOTTOM
	   VertexFormat(glm::vec3(-1, -1,  1), glm::vec3(0, 1, 1), glm::vec3(0, -1, 0), glm::vec2(1, 1)),
	   VertexFormat(glm::vec3(1, -1,  1), glm::vec3(0, 1, 1), glm::vec3(0, -1, 0), glm::vec2(0, 1)),
	   VertexFormat(glm::vec3(1, -1, -1), glm::vec3(0, 1, 1), glm::vec3(0, -1, 0), glm::vec2(0, 0)),
	   VertexFormat(glm::vec3(-1, -1, -1), glm::vec3(0, 1, 1), glm::vec3(0, -1, 0), glm::vec2(1, 0))
	};

	this->indices = {
		0, 1, 2,
		0, 2, 3,

		4, 5, 6,
		4, 6, 7,

		8, 9, 10,
		8, 10, 11,

		12, 14, 13,
		14, 12, 15,

		16, 18, 17,
		18, 16, 19,

		20, 22, 21,
		22, 20, 23
	};

}

Road::~Road()
{
}
