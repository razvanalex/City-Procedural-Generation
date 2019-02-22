#pragma once
#include <vector>
#include "Helper.h"
#include "Cylinder.h"
#include "Properties.h"

class Building {
public:
	Building(char *name, glm::vec3 position, float MaxW, float MaxL, float MaxH, 
		long seed, float topHeight, int numTops, float threashold, float numParts);
	~Building();

	char *name;

	glm::vec3 position;
	float MaxW;
	float MaxL;
	float MaxH;
	float numParts;
	long seed;
	float threashold;
	float topHeight;
	int numTops;
	float textureScale = 0.4f;

	// Spot lights
	static const int numSpotLights = 4;
	glm::vec3 positions[numSpotLights];
	float coneAngles[numSpotLights];
	glm::vec3 coneDirs[numSpotLights];
	glm::vec3 colors[numSpotLights];

	std::vector<Cylinder*> parts;
	void Generate();
	void CylinderBuilding();
	void ClassicBuilding();
	void TowerBuilding();
	void Draw(Shader *shader, Properties *prop, Texture2D* texture);
};