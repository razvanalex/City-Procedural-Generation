#include "Building.h"

Building::Building(char *name, glm::vec3 position, float MaxW, float MaxL, 
	float MaxH, long seed, float topHeight, int numTops, float threashold, float numParts)
{
	this->name = name;
	this->position = position;
	this->MaxH = MaxH;
	this->MaxL = MaxL;
	this->MaxW = MaxW;
	this->seed = seed;
	this->numParts = numParts;
	this->threashold = threashold;
	this->topHeight = topHeight;
	this->numTops = numTops;

	// Init parts
	parts = std::vector<Cylinder*>();

	for (int i = 0; i < this->numSpotLights; i++) {
		coneAngles[i] = 60;
		colors[i] = glm::vec3(Helper::random(0, 1), Helper::random(0, 1), Helper::random(0, 1));
	}

	this->Generate();
}

Building::~Building()
{
}

void Building::Generate()
{
	srand(seed);
	int type = Helper::random(0, 11);

	switch (type) {
	case 0: case 1: case 2: case 7: case 9: case 10:
		MaxH /= 2;
		ClassicBuilding();
		break;
	case 3: case 4: case 5: case 8:
		TowerBuilding();
		break;
	case 6:
		CylinderBuilding();
		break;
	default:
		break;
	}
}

void Building::CylinderBuilding()
{
	MaxW /= 2;
	MaxL /= 2;
	MaxH *= 2;

	// Generate base block
	float width = Helper::random(2 * MaxW / 3.0f, MaxW);
	float length = Helper::random(2 * MaxL / 3.0f, MaxL);
	float height = Helper::random(MaxH / (float)numParts, MaxH * (numParts - 1) / (float)numParts);

	float baseX = Helper::random(position.x - ((MaxW - width) / 2.0f), position.x + ((MaxW - width) / 2.0f));
	float baseZ = Helper::random(position.z - ((MaxL - length) / 2.0f), position.z + ((MaxL - length) / 2.0f));
	float baseY = height / 2.0f;

	char *partName = new char[50];

	numParts = (numParts <= 2) ? 2 : numParts;
	float tmp_h = MaxH;
	
	float distance = 1.0f;
	float angle = Helper::random(0.0f, glm::half_pi<float>());
	float X = distance * glm::cos(angle);
	float Z = distance * glm::sin(angle);

	// Spot lights
	//if (Helper::random(0.0f, 1) < 0.5) {
		positions[0] = glm::vec3(baseX - width * X, 0.01f, baseZ - length * Z);
		coneDirs[0] = glm::normalize(glm::vec3(X, height, Z));
	//}
	if (Helper::random(0.0f, 1) < 0.5) {
		positions[1] = glm::vec3(baseX + width * X, 0.01f, baseZ - length * Z);
		coneDirs[1] = glm::normalize(glm::vec3(-X, height, Z));
	}
	if (Helper::random(0.0f, 1) < 0.5) {
		positions[2] = glm::vec3(baseX + width * X, 0.01f, baseZ + length * Z);
		coneDirs[2] = glm::normalize(glm::vec3(-X, height, -Z));
	}
	if (Helper::random(0.0f, 1) < 0.5) {
		positions[3] = glm::vec3(baseX - width * X, 0.01f, baseZ + length * Z);
		coneDirs[3] = glm::normalize(glm::vec3(X, height, -Z));
	}

	for (int l = 0; l < numParts; l++) {
		sprintf(partName, "tower_%s_%d_base_%d", name, seed, l);
		parts.push_back(
			new Cylinder(partName,
				glm::vec3(baseX, baseY, baseZ),
				glm::vec3(0),
				glm::vec3(width, height, length),
				glm::vec3(1), 30, 1, 1, true, glm::vec2(textureScale, textureScale)));

		if (l == numParts - 1)
			break;

		tmp_h -= height;
		if (tmp_h <= 0) {
			break;
		}

		sprintf(partName, "tower_%s_%d_base_top_%d", name, seed, l);
		parts.push_back(
			new Cylinder(partName,
				glm::vec3(baseX, baseY + (height + topHeight) / 2.0f, baseZ),
				glm::vec3(0),
				glm::vec3(width * 1.05f, topHeight, length * 1.05f),
				glm::vec3(1), 30, 1, 1, false, glm::vec2(textureScale, textureScale)));

		baseY += height / 2.0f + topHeight;
		height = Helper::random(tmp_h / (float)(numParts - l - 1), tmp_h * (numParts - l - 2) / (float)(numParts - l - 1));
		baseY += height / 2.0f;

		float rnd = Helper::random(0, 1);
		width -= rnd < 0.5f ? 0.1f * width : 0;
		length -= rnd < 0.5f ? 0.1f * length : 0;
	}

	// Top of the building
	for (int i = 0; i < numTops; i++) {
		sprintf(partName, "tower_%s_%d_top_%d", name, seed, i);
		parts.push_back(
			new Cylinder(partName,
				glm::vec3(baseX, baseY + (height + topHeight * (2 * i + 1)) / 2.0f, baseZ),
				glm::vec3(0),
				glm::vec3(width - 0.05 * width * i, topHeight, length - 0.05 * i * length),
				glm::vec3(1), 30, 1, 1, false, glm::vec2(textureScale, textureScale)));
	}

	int numVents = Helper::random(1, 10);

	//Ventilation
	for (int i = 0; i < numVents; i++) {
		sprintf(partName, "tower_%s_%d_vents_%d", name, seed, i);
		float w = Helper::random(width * 0.05f, width * 0.1f);
		float l = Helper::random(length * 0.05f, length * 0.1f);
		float h = Helper::random(topHeight / 5.0f, topHeight / 2.0f);

		float topWidth = width - 0.05f * width * (numTops - 1);
		float topLength = length - 0.05f * width * (numTops - 1);

		float x = Helper::random(baseX - (topWidth - w) / 2.0f, baseX + (topWidth - w) / 2.0f);
		float z = Helper::random(baseZ - (topLength - w) / 2.0f, baseZ + (topLength - w) / 2.0f);
		float y = baseY + (height + topHeight * 2 * numTops + h) / 2.0f;

		parts.push_back(
			new Cylinder(partName,
				glm::vec3(x, y, z),
				glm::vec3(0),
				glm::vec3(w, h, l),
				glm::vec3(1), 4, 1, 1, false, glm::vec2(textureScale, textureScale)));
	}

	delete[] partName;
}


void Building::TowerBuilding()
{
	MaxW /= glm::sqrt(2);
	MaxL /= glm::sqrt(2);
	MaxH *= 2;

	// Generate base block
	float width = Helper::random(2 * MaxW / 3.0f, MaxW);
	float length = Helper::random(2 * MaxL / 3.0f, MaxL);
	float height =  Helper::random(MaxH / (float)numParts, MaxH * (numParts - 1) / (float)numParts);

	float baseX = Helper::random(position.x - ((MaxW - width) / 2.0f), position.x + ((MaxW - width) / 2.0f));
	float baseZ = Helper::random(position.z - ((MaxL - length) / 2.0f), position.z + ((MaxL - length) / 2.0f));
	float baseY = height / 2.0f;

	float distance = 1.0f;
	float angle = Helper::random(0.0f, glm::half_pi<float>());
	float X = distance * glm::cos(angle);
	float Z = distance * glm::sin(angle);

	// Spot lights
	//if (Helper::random(0.0f, 1) < 0.5) {
		positions[0] = glm::vec3(baseX - width * X, 0.01f, baseZ - length * Z);
		coneDirs[0] = glm::normalize(glm::vec3(X, height, Z));
	//}
	if (Helper::random(0.0f, 1) < 0.5) {
		positions[1] = glm::vec3(baseX + width * X, 0.01f, baseZ - length * Z);
		coneDirs[1] = glm::normalize(glm::vec3(-X, height, Z));
	}
	if (Helper::random(0.0f, 1) < 0.5) {
		positions[2] = glm::vec3(baseX + width * X, 0.01f, baseZ + length * Z);
		coneDirs[2] = glm::normalize(glm::vec3(-X, height, -Z));
	}
	if (Helper::random(0.0f, 1) < 0.5) {
		positions[3] = glm::vec3(baseX - width * X, 0.01f, baseZ + length * Z);
		coneDirs[3] = glm::normalize(glm::vec3(X, height, -Z));
	}


	char *partName = new char[50];

	numParts = (numParts <= 2) ? 2 : numParts;
	float tmp_h = MaxH;

	for (int l = 0; l < numParts; l++) {
		sprintf(partName, "tower_%s_%d_base_%d", name, seed, l);
		parts.push_back(
			new Cylinder(partName,
				glm::vec3(baseX, baseY, baseZ),
				glm::vec3(0),
				glm::vec3(width, height, length),
				glm::vec3(1), 4, 1, 1, true, glm::vec2(textureScale, textureScale)));

		if (l == numParts - 1)
			break;

		tmp_h -= height;
		if (tmp_h <= 0) {
			break;
		}

		sprintf(partName, "tower_%s_%d_base_top_%d", name, seed, l);
		parts.push_back(
			new Cylinder(partName,
				glm::vec3(baseX, baseY + (height + topHeight) / 2.0f, baseZ),
				glm::vec3(0),
				glm::vec3(width * 1.05f, topHeight, length * 1.05f),
				glm::vec3(1), 4, 1, 1, false, glm::vec2(textureScale, textureScale)));

		baseY += height / 2.0f + topHeight;
		height = Helper::random(tmp_h / (float)(numParts - l - 1), tmp_h * (numParts - l - 2) / (float)(numParts - l - 1));
		baseY += height / 2.0f;

		float rnd = Helper::random(0, 1);
		width -= rnd < 0.5f ? 0.1f * width : 0;
		length -= rnd < 0.5f ? 0.1f * length : 0;
	}

	// Top of the building
	for (int i = 0; i < numTops; i++) {
		sprintf(partName, "tower_%s_%d_top_%d", name, seed, i);
		parts.push_back(
			new Cylinder(partName,
				glm::vec3(baseX, baseY + (height + topHeight * (2 * i + 1)) / 2.0f, baseZ),
				glm::vec3(0),
				glm::vec3(width - 0.05 * width * i, topHeight, length - 0.05 * i * length),
				glm::vec3(1), 4, 1, 1, false, glm::vec2(textureScale, textureScale)));
	}

	int numVents = Helper::random(1, 10);

	//Ventilation
	for (int i = 0; i < numVents; i++) {
		sprintf(partName, "tower_%s_%d_vents_%d", name, seed, i);
		float w = Helper::random(width * 0.05f, width * 0.1f);
		float l = Helper::random(length * 0.05f, length * 0.1f);
		float h = Helper::random(topHeight / 5.0f, topHeight / 2.0f);

		float topWidth = width - 0.05f * width * (numTops - 1);
		float topLength = length - 0.05f * width * (numTops - 1);

		float x = Helper::random(baseX - (topWidth - w) / 2.0f, baseX + (topWidth - w) / 2.0f);
		float z = Helper::random(baseZ - (topLength - w) / 2.0f, baseZ + (topLength - w) / 2.0f);
		float y = baseY + (height + topHeight * 2 * numTops + h) / 2.0f;

		parts.push_back(
			new Cylinder(partName,
				glm::vec3(x, y, z),
				glm::vec3(0),
				glm::vec3(w, h, l),
				glm::vec3(1), 4, 1, 1, false, glm::vec2(textureScale, textureScale)));
	}

	delete[] partName;
}

void Building::Draw(Shader *shader, Properties *prop, Texture2D* texture)
{
	for (auto p : this->parts) {
		prop->Set("Model", new Mat4Attribute(p->GetModelMatrix()));

		if (p->textureEnable) {
			prop->Set("textureEnable", new IntAttribute(1))
				->Set("texture", new TextureAttribute(texture));
		}
		else {
			prop->Set("textureEnable", new IntAttribute(0));
		}

		p->RenderMesh(shader, prop);
	}
}

void Building::ClassicBuilding() 
{
	MaxW /= glm::sqrt(2);
	MaxL /= glm::sqrt(2);

	// Generate base block
	float width = Helper::random(MaxW / 2.0f, MaxW);
	float length = Helper::random(MaxL / 2.0f, MaxL);
	float height = MaxH;

	float baseX = Helper::random(position.x - ((MaxW - width) / 2.0f), position.x + ((MaxW - width) / 2.0f));
	float baseZ = Helper::random(position.z - ((MaxL - length) / 2.0f), position.z + ((MaxL - length) / 2.0f));
	float baseY = height / 2.0f;


	float distance = 1.0f;
	float angle = Helper::random(0.0f, glm::half_pi<float>());
	float X = distance * glm::cos(angle);
	float Z = distance * glm::sin(angle);

	// Spot lights
	//if (Helper::random(0.0f, 1) < 0.5) {
		positions[0] = glm::vec3(baseX - width * X, 0.01f, baseZ - length * Z);
		coneDirs[0] = glm::normalize(glm::vec3(X, height, Z));
	//}
	if (Helper::random(0.0f, 1) < 0.5) {
		positions[1] = glm::vec3(baseX + width * X, 0.01f, baseZ - length * Z);
		coneDirs[1] = glm::normalize(glm::vec3(-X, height, Z));
	}
	if (Helper::random(0.0f, 1) < 0.5) {
		positions[2] = glm::vec3(baseX + width * X, 0.01f, baseZ + length * Z);
		coneDirs[2] = glm::normalize(glm::vec3(-X, height, -Z));
	}
	if (Helper::random(0.0f, 1) < 0.5) {
		positions[3] = glm::vec3(baseX - width * X, 0.01f, baseZ + length * Z);
		coneDirs[3] = glm::normalize(glm::vec3(X, height, -Z));
	}


	char *partName = new char[50];
	sprintf(partName, "classic_%s_%d_base", name, seed);
	parts.push_back(
		new Cylinder(partName,
			glm::vec3(baseX, baseY, baseZ),
			glm::vec3(0),
			glm::vec3(width, height, length),
			glm::vec3(1), 4, 1, 1, true, glm::vec2(textureScale, textureScale)));

	// Top of the building
	for (int i = 0; i < numTops; i++) {
		sprintf(partName, "classic_%s_%d_top_%d", name, seed, i);
		parts.push_back(
			new Cylinder(partName,
				glm::vec3(baseX, baseY + (height + topHeight * (2 * i + 1)) / 2.0f, baseZ),
				glm::vec3(0),
				glm::vec3(width - 0.05 * width * i, topHeight, length - 0.05 * i * length),
				glm::vec3(1), 4, 1, 1, false, glm::vec2(textureScale, textureScale)));
	}

	int numVents = Helper::random(1, 10);

	// Ventilation
	for (int i = 0; i < numVents; i++) {
		sprintf(partName, "classic_%s_%d_vents_%d", name, seed, i);
		float w = Helper::random(width * 0.05f, width * 0.1f);
		float l = Helper::random(length * 0.05f, length * 0.1f);
		float h = Helper::random(topHeight / 5.0f, topHeight / 2.0f);

		float topWidth = width - 0.05f * width * (numTops - 1);
		float topLength = length - 0.05f * width * (numTops - 1);

		float x = Helper::random(baseX - (topWidth - w) / 2.0f, baseX + (topWidth - w) / 2.0f);
		float z = Helper::random(baseZ - (topLength - w) / 2.0f, baseZ + (topLength - w) / 2.0f);
		float y = baseY + (height + topHeight * 2 * numTops + h) / 2.0f;

		parts.push_back(
			new Cylinder(partName,
				glm::vec3(x, y, z),
				glm::vec3(0),
				glm::vec3(w, h, l),
				glm::vec3(1), 4, 1, 1, false, glm::vec2(textureScale, textureScale)));
	}

	// Generate parts
	for (int i = 0; i < numParts; i++) {
		float w = Helper::random(width / 2.0f, width);
		float l = Helper::random(length / 2.0f, length);
		float h = Helper::random(height / 4.0f, height);

		float pX = Helper::random(position.x - (width - w) / 2.0f, position.x + (width - w) / 2.0f);
		float pZ = Helper::random(position.z - (length - l) / 2.0f, position.z + (length - l) / 2.0f);
		float pY = h / 2.0f;

		if (glm::abs(baseX - pX - w / 2.0f) < threashold
			|| glm::abs(baseZ - pZ - l / 2.0f) < threashold)
		{
			continue;
		}

		sprintf(partName, "classic_%s_%d_%d_base", name, seed, i);

		parts.push_back(
			new Cylinder(partName,
				glm::vec3(pX, pY, pZ),
				glm::vec3(0),
				glm::vec3(w, h, l),
				glm::vec3(1), 4, 1, 1, true, glm::vec2(textureScale, textureScale)));

		sprintf(partName, "classic_%s_%d_%d_top", name, seed, i);
		parts.push_back(
			new Cylinder(partName,
				glm::vec3(pX, pY + (h + topHeight) / 2.0f, pZ),
				glm::vec3(0),
				glm::vec3(w, topHeight, l),
				glm::vec3(1), 4, 1, 1, false, glm::vec2(textureScale, textureScale)));
	}

	delete[] partName;
}