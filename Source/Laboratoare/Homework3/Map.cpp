#include "Map.h"

Map::Map(float scale, int width, int height, int numStreets, 
	int maxStreetLength, int minDistance)
{
	this->scale = scale;
	this->width = width;
	this->height = height;
	this->numStreets = numStreets;
	this->maxStreetLength = maxStreetLength;
	this->minDistance = minDistance;
	this->buildings = std::vector<Building*>();
	this->roads = std::vector<Road*>();
	this->trees = std::vector<Tree*>();
	this->cars = std::vector<Car*>();
	this->streetLights = std::vector<StreetLight*>();

	srand(time(NULL));

	map = new char*[height];
	for (int i = 0; i < height; i++) {
		map[i] = new char[width];
		memset(map[i], '.', width * sizeof(char));
	}

	Init();
	rectangles();
	streetRectangles();
}

Map::~Map()
{
	for (int i = 0; i < height; i++)
		delete[] map[i];
	delete[] map;
}


void Map::rectangles()
{
	if (height <= 0 || width <= 0)
		return;

	// Init a visited matrix with 0
	int **visited = new int*[height];
	for (int i = 0; i < height; i++) {
		visited[i] = new int[width];
		memset(visited[i], 0, width * sizeof(int));
	}

	int notToSkip = '.';
	int isCompleteLine;

	// Find a corner of a rectangle
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (visited[i][j] || map[i][j] != notToSkip)
				continue;

			int x = j, y = i;

			// A rectangle is found
			while (x + 1 < width && map[y][x + 1] == notToSkip && !visited[y][x + 1]) {
				visited[y][x++] = 1;
			}
			visited[y][x] = 1;

			do {
				if (y + 1 >= height)
					break;

				isCompleteLine = 1;
				for (int l = j; l <= x; l++) {
					if (map[y + 1][l] != notToSkip || visited[y + 1][x]) {
						isCompleteLine = 0;
						break;
					}
				}
				if (isCompleteLine) {
					for (int l = j; l <= x; l++) {
						visited[y + 1][l] = 1;
					}
					y++;
				}
			} while (isCompleteLine && y < height);

			if (!generateBuildings(j, i, x, y)) {
				generateTrees(j, i, x, y);
			}
		}
	}

	// Print the map	
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			printf("%c ", map[i][j]);
		}
		printf("\n");
	}
}

void Map::generateTrees(int startX, int startY, int stopX, int stopY) 
{
	float sX = (float)startX;
	float sY = (float)startY;
	float eX = (float)stopX;
	float eY = (float)stopY;

	float maxWidth = eX - sX + 1;
	float maxLength = eY - sY + 1;

	glm::vec3 position = glm::vec3(sX + (maxWidth - this->width) / 2.0f, 0, sY + (maxLength - this->height) / 2.0f);

	// Generate trees

	// Number of trees
	int numX = maxWidth / minWidthTree;
	int numY = maxLength / minWidthTree;

	// Size of trees
	float width = maxWidth / (float)numX;
	float length = maxLength / (float)numY;

	glm::vec3 cornerPosition = position - glm::vec3(maxWidth / 2.0f, 0, maxLength / 2.0f);

	// Generate trees in cell
	for (int i = 0; i < numX; i++) {
		for (int j = 0; j < numY; j++) {
			if (Helper::random(0, 1) < 0.3f)
				continue;

			float maxHeight = Helper::random(0.5f, 1.25f);

			// Name of the tree
			int num = trees.size();
			char name[50];
			sprintf(name, "tree_%d", num);

			// Delta position w.r.t. cornerPosition
			glm::vec3 deltaPos = glm::vec3((i + 0.5f) * width, 0, (j + 0.5f) * length);

			// Random rotation
			glm::vec3 rotation = glm::vec3(0, Helper::random(0, glm::pi<float>()), 0);

			// Add tree
			trees.push_back(new Tree(name, cornerPosition + deltaPos, rotation,
				glm::vec3(width, width, maxHeight), glm::vec3(1)));
		}
	}
}

void Map::generateCars(int startX, int startY, int stopX, int stopY, char type)
{
	float sX = (float)startX;
	float sY = (float)startY;
	float eX = (float)stopX;
	float eY = (float)stopY;

	float maxWidth = eX - sX + 1;
	float maxLength = eY - sY + 1;

	glm::vec3 position = glm::vec3(sX + (maxWidth - this->width) / 2.0f, 0, sY + (maxLength - this->height) / 2.0f);
	glm::vec3 rotation = glm::vec3(0);

	if (maxWidth > 1 || type == 'h') {
		for (int i = 0; i < maxWidth; i++) {
			// Compute rotation
			rotation = glm::vec3(0, glm::half_pi<float>(), 0);
			
			// Compute position
			position = glm::vec3(sX + (maxWidth - this->width) / 2.0f, 0, sY + (maxLength - this->height) / 2.0f);
			position += glm::vec3((maxWidth - 1) / 2.0f - i, 0, 0);

			type = (map[startY][startX + i] == 'c') ? 'c' : type;
			generateStreetLights(position, rotation, type);
			if (Helper::random(0, 1) < 1.0f - traffic)
				continue;

			rotation = glm::vec3(0, 0, glm::half_pi<float>());
			// Choose lane
			if (Helper::random(0, 1) < 0.5) {
				rotation += glm::vec3(0, 0, glm::pi<float>());
				position -= glm::vec3(0, 0, 0.25f);
			}
			else {
				position += glm::vec3(0, 0, 0.25f);
			}

			// Add car
			cars.push_back(new Car("car", position, rotation, glm::vec3(.02), glm::vec3(1)));
		}
	}
	else {
		for (int i = 0; i < maxLength; i++) {
			// Compute rotation
			rotation = glm::vec3(0, 0, 0);

			// Compute position
			position = glm::vec3(sX + (maxWidth - this->width) / 2.0f, 0, sY + (maxLength - this->height) / 2.0f);
			position += glm::vec3(0, 0, (maxLength - 1) / 2.0f - i);

			type = map[stopY - i][startX];
			generateStreetLights(position, rotation, type);
			if (Helper::random(0, 1) < 1.0f - traffic)
				continue;

			// Choose lane
			if (Helper::random(0, 1) < 0.5) {
				rotation += glm::vec3(0, 0, glm::pi<float>());
				position -= glm::vec3(0.25f, 0, 0);
			}
			else {
				position += glm::vec3(0.25f, 0, 0);
			}

			// Add car
			cars.push_back(new Car("car", position, rotation, glm::vec3(.02), glm::vec3(1)));
		}
	}
	
}

void Map::generateStreetLights(glm::vec3 position, glm::vec3 rotation, char type)
{
	if (type == 'h') {
		// Add light
		streetLights.push_back(new StreetLight("lights", 
			position - glm::vec3(0, 0, 0.5f), 
			rotation - glm::vec3(0, glm::half_pi<float>(), glm::half_pi<float>()),
			glm::vec3(.02), glm::vec3(1)));

		// Add light
		streetLights.push_back(new StreetLight("lights", 
			position + glm::vec3(0, 0, 0.5f),
			rotation - glm::vec3(0, glm::half_pi<float>(), -glm::half_pi<float>()),
			glm::vec3(.02), glm::vec3(1)));
	}
	else if (type == 'v') {
		// Add light
		streetLights.push_back(new StreetLight("lights",
			position - glm::vec3(0.5f, 0, 0),
			rotation - glm::vec3(0, 0, 0),
			glm::vec3(.02), glm::vec3(1)));

		// Add light
		streetLights.push_back(new StreetLight("lights",
			position + glm::vec3(0.5f, 0, 0),
			rotation - glm::vec3(0, 0, -glm::pi<float>()),
			glm::vec3(.02), glm::vec3(1)));
	}
}


void Map::streetRectangles()
{
	if (height <= 0 || width <= 0)
		return;

	// Init a visited matrix with 0
	int **visited = new int*[height];
	for (int i = 0; i < height; i++) {
		visited[i] = new int[width];
		memset(visited[i], 0, width * sizeof(int));
	}

	int toSkip = '.';
	int isCompleteLine;

	// Find a corner of a rectangle
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (visited[i][j] || map[i][j] == toSkip || map[i][j] == 't')
				continue;

			int x = j, y = i;

			// A rectangle is found
			while (x + 1 < width && map[y][x + 1] != toSkip && map[y][x + 1] != 't' && !visited[y][x + 1]) {
				visited[y][x++] = 1;
			}
			visited[y][x] = 1;

			do {
				if (y + 1 >= height)
					break;

				isCompleteLine = 1;
				for (int l = j; l <= x; l++) {
					if (map[y + 1][l] == toSkip || map[y + 1][l] == 't'  || visited[y + 1][x]) {
						isCompleteLine = 0;
						break;
					}
				}
				if (isCompleteLine) {
					for (int l = j; l <= x; l++) {
						visited[y + 1][l] = 1;
					}
					y++;
				}
			} while (isCompleteLine && y < height);

			generateRoads(j, i, x, y, map[i][j]);
		}
	}
}

void Map::generateRoads(int startX, int startY, int stopX, int stopY, char type)
{
	float sX = (float)startX;
	float sY = (float)startY;
	float eX = (float)stopX;
	float eY = (float)stopY;

	// Name of the road
	int num = roads.size();
	char name[50];
	sprintf(name, "road_%d", num);

	float maxHeight = 0.01f;
	float maxWidth = eX - sX + 1;
	float maxLength = eY - sY + 1;

	glm::vec3 position = glm::vec3(sX + (maxWidth - this->width) / 2.0f, 0, sY + (maxLength - this->height) / 2.0f);
	glm::vec3 rotation = glm::vec3(0);

	if (maxWidth > maxLength) {
		float aux = maxWidth;
		maxWidth = maxLength;
		maxLength = aux;
		rotation.y += glm::half_pi<float>();
	}
	else if (maxWidth == maxLength) {
		if (type == 'h') {
			float aux = maxWidth;
			maxWidth = maxLength;
			maxLength = aux;
			rotation.y += glm::half_pi<float>();
		}
	}

	// Generate cars on the road
	generateCars(startX, startY, stopX, stopY, type);

	// Add road
	roads.push_back(new Road(name, position, rotation, glm::vec3(maxWidth/ 2.0f, maxHeight, maxLength / 2.0f), glm::vec3(0)));
}

void Map::generateCell(glm::vec3 position, float maxWidth, float maxLength,
	float maxHeight, long seed, int numTops, int numParts)
{
	// Number of buildings
	int numX = maxWidth / minWidth;
	int numY = maxLength / minLength;

	// Size of buildings
	float width = maxWidth / (float)numX;
	float length = maxLength / (float)numY;
	float height = Helper::random(maxHeight / 2.0f, maxHeight);

	glm::vec3 cornerPosition = position - glm::vec3(maxWidth / 2.0f, 0, maxLength / 2.0f);
	srand(seed);

	// Generate buildings in cell
	for (int i = 0; i < numX; i++) {
		for (int j = 0; j < numY; j++) {
			// Random seed
			long newSeed = Helper::random(0, RAND_MAX);

			// Name of the bilding
			int num = buildings.size();
			char name[50];
			sprintf(name, "building_%d", num);

			// Delta position w.r.t. cornerPosition
			glm::vec3 deltaPos = glm::vec3((i + 0.5f) * width, 0, (j + 0.5f) * length);

			// Add building
			buildings.push_back(new Building(name, cornerPosition + deltaPos, width, length,
				maxHeight, newSeed, 0.1f, numTops, 0.01f, numParts));
		}
	}
}

bool Map::generateBuildings(int startX, int startY, int stopX, int stopY)
{
	float sX = (float)startX;
	float sY = (float)startY;
	float eX = (float)stopX;
	float eY = (float)stopY;
	
	int maxHeight = Helper::random(1, 5);
	float maxWidth = eX - sX + 1;
	float maxLength = eY - sY + 1;

	int numTops = Helper::random(1, 4);
	int numParts = 5;
	glm::vec3 position = glm::vec3(sX + (maxWidth - this->width) / 2.0f, 0, sY + (maxLength - this->height) / 2.0f);
	long seed = Helper::random(0, RAND_MAX);

	if ((int)maxWidth / (int)minWidth == 0 || (int)maxLength / (int)minLength == 0)
		return false;

	generateCell(position, maxWidth, maxLength, maxHeight, seed, numTops, numParts);

	return true;
}

void Map::tagMap(int rndLength, int rndX, int rndY, int rndStart, 
	int lastOrientation, int tag)
{
	// Orientation: Horizontal, Vertical
	int orientations[2][2] = { {-1, 0}, {0, -1}, };

	// "Draw" the road
	int x = 0, y = 0;
	for (int p = 0; p < rndLength; p++) {
		x = rndX + orientations[lastOrientation][0] * p;
		y = rndY + orientations[lastOrientation][1] * p;

		if (lastOrientation == 0)
			x += rndStart;
		else if (lastOrientation == 1)
			y += rndStart;

		if (x >= width || y >= height || x < 0 || y < 0)
			break;

		if (map[y][x] == 'h' || map[y][x] == 'v')
			map[y][x] = 'c';
		else
			map[y][x] = tag;
	}
}

void Map::checkMap(int rndLength, int rndX, int rndY, int rndStart,
	int lastOrientation, int minDistance, int tag, int *slots, int *sizeSlots)
{
	// Orientation: Horizontal, Vertical
	int orientations[2][2] = { {-1, 0}, {0, -1} };

	// "Draw" the road
	int x = 0, y = 0;
	for (int p = 0; p < rndLength; p++) {
		x = rndX + orientations[lastOrientation][0] * p;
		y = rndY + orientations[lastOrientation][1] * p;

		if (lastOrientation == 0)
			x += rndStart;
		else if (lastOrientation == 1)
			y += rndStart;

		if (x >= width || y >= height || x < 0 || y < 0)
			break;

		int startX = Helper::maxF(x - minDistance, 0);
		int endX = Helper::minF(x + minDistance + 1, width);
		int startY = Helper::maxF(y - minDistance, 0);
		int endY = Helper::minF(y + minDistance + 1, height);

		int isOk = 1;
		for (int i = startY; i < endY; i++)
			for (int j = startX; j < endX; j++) {
				if (map[i][j] == tag || map[i][j] == 'c') {
					isOk = 0;
					break;
				}
			}

		if (isOk) {
			if (lastOrientation == 0) {
				slots[(*sizeSlots)++] = x;
			}
			else if (lastOrientation == 1) {
				slots[(*sizeSlots)++] = y;
			}
		}
	}
}

void Map::Init()
{
	srand(time(NULL));
	
	// Orientation: Horizontal = 0, Vertical = 1;
	int lastOrientation = rand() % 2;

	// Set random starting position
	int rndX = rand() % (width - 2) + 1;
	int rndY = rand() % (height - 2) + 1;

	// Generate the roads
	for (int _numStreets = numStreets; _numStreets > 0; _numStreets--) {
		int rndLength = (rand() % maxStreetLength) + 2 * minDistance;
		lastOrientation = 1 - lastOrientation;
		int maxLimit = Helper::minF((lastOrientation ? height - rndY : width - rndX), rndLength);
		int rndStart = Helper::random(0, maxLimit);
		int maxSizeSlots = maxStreetLength + 2 * minDistance, sizeSlots = 0;
		int *slots = new int[maxSizeSlots];
				
		int crtTag = (lastOrientation == 0 ? 'h' : 'v');
		int toAvoid = (lastOrientation == 0 ? 'v' : 'h');
		
		checkMap(rndLength, rndX, rndY, rndStart, lastOrientation, 
			minDistance, toAvoid, slots, &sizeSlots);

		tagMap(rndLength, rndX, rndY, rndStart, lastOrientation, crtTag);
		
		if (sizeSlots == 0) {
			checkMap(maxStreetLength, rndX, rndY, rndStart, lastOrientation, 
				minDistance, toAvoid, slots, &sizeSlots);
			
			if (sizeSlots == 0)
				break;
			continue;
		}
			
		// Generate a random position on the last street
		if (lastOrientation == 0) {
			rndX = slots[(int)Helper::random(0, sizeSlots)];
		}
		else if (lastOrientation == 1) {
			rndY = slots[(int)Helper::random(0, sizeSlots)];
		}

		delete[] slots;
	}
}
