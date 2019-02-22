#pragma once
#include <cstring>
#include "Helper.h"
#include <include/glm.h>
#include "Building.h"
#include "Road.h"
#include "Tree.h"
#include "Car.h"
#include "StreetLight.h";

class Map {
public:
	Map(float scale, int width, int height, int numStreets, int maxStreetLength, int minDistance);
	~Map();

public:
	void Init();
	std::vector<Building*> buildings;
	std::vector<Road*> roads;
	std::vector<Tree*> trees;
	std::vector<Car*> cars;
	std::vector<StreetLight*> streetLights;

private:
	float scale;
	int width;
	int height;
	int numStreets;
	int maxStreetLength;
	int minDistance;
	char **map;

	float minWidth = 2;
	float minLength = 2;
	float minWidthTree = 0.5f;
	float traffic = 0.4f;

	void rectangles();
	void streetRectangles();
	void generateRoads(int startX, int startY, int stopX, int stopY, char type);
	bool generateBuildings(int startX, int startY, int stopX, int stopY);
	void tagMap(int rndLength, int rndX, int rndY, int rndStart, int lastOrientation, int tag);
	void checkMap(int rndLength, int rndX, int rndY, int rndStart,
		int lastOrientation, int minDistance, int tag, int *slots, int *sizeSlots);

	void generateCell(glm::vec3 position, float maxWidthm, float maxLength,
		float maxHeight, long seed, int numTops, int numParts);

	void generateTrees(int startX, int startY, int stopX, int stopY);
	void generateCars(int startX, int startY, int stopX, int stopY, char type);
	void generateStreetLights(glm::vec3 position, glm::vec3 rotation, char type);

};