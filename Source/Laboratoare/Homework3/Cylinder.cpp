#include "Cylinder.h"

Cylinder::Cylinder(char * name, glm::vec3 position, glm::vec3 rotation,
	glm::vec3 scale, glm::vec3 color, int sharpness, float radius1,
	float radius2, bool textureEnable, glm::vec2 textureScale) : Object3D(name, position, glm::vec3(0), glm::vec3(1), color)
{
	this->sharpness = sharpness >= 3 ? sharpness : 3;
	this->radius1 = radius1;
	this->radius2 = radius2;
	this->textureScale = textureScale;

	this->rotation = rotation;
	this->scale = scale;
	this->textureEnable = textureEnable;

	UpdateModelMatrix();
	Init();
}

Cylinder::~Cylinder()
{
}

void Cylinder::Init()
{
	float x, y, z;
	unsigned short crtIndex = 1, centerIndex = 0;
	float step = 2.0f * glm::pi<float>() / (float)sharpness;

	std::vector<int> toSkip = std::vector<int>();
	int index;

	// Generate the 2 circles
	for (int c = 0; c < 2; c++) {
		float radius = (c == 0) ? radius1 : radius2;

		x = 0;
		y = (c == 0) ? -0.5f : 0.5f;
		z = 0;
		this->vertices.push_back(VertexFormat(glm::vec3(x, y, z), color,
			glm::vec3(0, y, 0), glm::vec2(-1.0f)));

		index = 0;
		for (float i = 0; i < 2 * glm::pi<float>(); i += step) {
			index++;
			if (c == 0 && skipEdge) {
				i += glm::pi<float>() / 2.0f - step;
				toSkip.push_back(index);
				skipEdge = false;
				continue;
			}
			else if (c == 1) {
				bool skip = false;

				for (auto el : toSkip) {
					if (index == el) {
						skip = true;
						i += glm::pi<float>() / 2.0f - step;
						break;
					}
				}

				if (skip)
					continue;
			}

			float fi = i + glm::pi<float>() / (float)sharpness;
			x = radius * cos(fi);
			z = radius * sin(fi);

			this->vertices.push_back(VertexFormat(glm::vec3(x, y, z), color,
				glm::vec3(0, y, 0), glm::vec2(-1.0f)));

			if (crtIndex >= centerIndex + 2) {
				if (c == 1) {
					this->indices.push_back(crtIndex);
					this->indices.push_back(crtIndex - 1);
					this->indices.push_back(centerIndex);
				}
				else {
					this->indices.push_back(centerIndex);
					this->indices.push_back(crtIndex - 1);
					this->indices.push_back(crtIndex);
				}
			}

			crtIndex++;
		}

		if (c == 1) {
			this->indices.push_back(centerIndex + 1);
			this->indices.push_back(crtIndex - 1);
			this->indices.push_back(centerIndex);
		}
		else {
			this->indices.push_back(centerIndex);
			this->indices.push_back(crtIndex - 1);
			this->indices.push_back(centerIndex + 1);
		}
		centerIndex = crtIndex++;
	}

	// Generate the side of the cylinder
	crtIndex--;
	index = 0;
	float angle = step;
	bool skip = false;
	float end = 2 * glm::pi<float>();
	for (float i = 0; i < end; i += step)
	{
		index++;
		if (skip) {
			skip = false;
			continue;
		}

		for (auto el : toSkip) {
			if (index == el && el != 1) {
				i += glm::pi<float>() / 2.0f - step;
				angle = glm::pi<float>() / 2.0f;
				skip = true;
				break;
			}
			else if (index == el && el == 1) {
				i -= 2 * step - glm::pi<float>() / 2.0f;
				angle = 2 * step;
				end -= 2 * step - glm::pi<float>() / 2.0f;
				skip = true;
				break;
			}
		}

		float fi = i + glm::pi<float>() / (float)sharpness;

		glm::vec3 v1 = glm::vec3(radius1 * cos(fi), -0.5f, radius1 * sin(fi));
		glm::vec3 v2 = glm::vec3(radius1 * cos(fi + angle), -0.5f, radius1 * sin(fi + angle));
		glm::vec3 v3 = glm::vec3(radius2 * cos(fi + angle), 0.5f, radius2 * sin(fi + angle));
		glm::vec3 v4 = glm::vec3(radius2 * cos(fi), 0.5f, radius2 * sin(fi));

		glm::vec3 normal = glm::cross(v1 - v2, v3 - v2);
		float sizeX = glm::length(Transform3D::Scale(scale.x, scale.y, scale.z) * glm::vec4(v1 - v2, 1));
		float sideScaleTex = sizeX / textureScale.x;
		float heightScaleTex = scale.y / textureScale.y;

		// Set vertices
		this->vertices.push_back(VertexFormat(v1, color, normal, glm::vec2(sideScaleTex, heightScaleTex)));
		this->vertices.push_back(VertexFormat(v2, color, normal, glm::vec2(0.0f, heightScaleTex)));
		this->vertices.push_back(VertexFormat(v3, color, normal, glm::vec2(0.0f, 0.0f)));
		this->vertices.push_back(VertexFormat(v4, color, normal, glm::vec2(sideScaleTex, 0.0f)));

		// Set indices
		this->indices.push_back(crtIndex);
		this->indices.push_back(crtIndex + 2);
		this->indices.push_back(crtIndex + 1);

		this->indices.push_back(crtIndex);
		this->indices.push_back(crtIndex + 3);
		this->indices.push_back(crtIndex + 2);

		crtIndex += 4;
		angle = step;
	}
}
