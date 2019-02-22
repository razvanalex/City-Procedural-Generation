#include "Homework.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

#include "Properties.h"
#include <ctime>
#include "Map.h"
#include "Ground.h"

Homework::Homework()
{
}

Homework::~Homework()
{
}

Map *map;
std::unordered_map<std::string, Texture2D*> mapTextures;
Properties *prop;
Ground *ground;

void Homework::Init()
{
	camera = new Camera(glm::vec3(1, 2, 0), glm::vec3(1), glm::vec3(0, 1, 0));
	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
	map = new Map(1, 40, 40, 100, 40, 5);
	
	ground = new Ground("ground", glm::vec3(0), glm::vec3(0), glm::vec3(40, 1, 40), glm::vec3(1));

	// Load textures
	{
		const string textureLoc = "Resources/Textures/";
		{
			Texture2D* texture = new Texture2D();
			texture->Load2D((textureLoc + "Road_Lanes.png").c_str(), GL_REPEAT);
			mapTextures["road_lanes"] = texture;
		}

		{
			Texture2D* texture = new Texture2D();
			texture->Load2D((textureLoc + "window.png").c_str(), GL_REPEAT);
			mapTextures["window"] = texture;
		}

		{
			Texture2D* texture = new Texture2D();
			texture->Load2D((textureLoc + "BroadleafBark.tga").c_str(), GL_REPEAT);
			mapTextures["BroadleafBark"] = texture;
		}
		 
		{
			Texture2D* texture = new Texture2D();
			texture->Load2D((textureLoc + "snow.png").c_str(), GL_REPEAT);
			mapTextures["snow"] = texture;
		}
		{
			Texture2D* texture = new Texture2D();
			texture->Load2D((RESOURCE_PATH::MODELS + "\/Characters\/Archer\/Akai_E_Espiritu.fbm\/akai_diffuse.png").c_str(), GL_REPEAT);
			mapTextures["archer"] = texture;
		}

		
	}

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader *shader = new Shader("Lighting");
		shader->AddShader("Source/Laboratoare/Homework3/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Homework3/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// Init properties of shaders
	prop = (new Properties(shaders["Lighting"]))
		->Set("Projection", new Mat4Attribute(projectionMatrix))
		->Set("color", new Vec3Attribute(glm::vec3(0.25f)))
		->Set("light_direction", new Vec3Attribute(glm::vec3(2, -6, -3)))
		->Set("material_kd", new FloatAttribute(1))
		->Set("material_ks", new FloatAttribute(0.5f))
		->Set("material_shininess", new IntAttribute(30));
}

void Homework::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0.529f, 0.808f, 0.922f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Homework::Update(float deltaTimeSeconds)
{
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	Draw(deltaTimeSeconds);
}

void Homework::Draw(float deltaTimeSeconds)
{
	prop->Set("View", new Mat4Attribute(camera->GetViewMatrix()))
		->Set("eye_position", new Vec3Attribute(camera->GetPosition()))
		->Set("light_direction", new Vec3Attribute(glm::vec3(2, -6, -3)))
		->Set("material_kd", new FloatAttribute(1))
		->Set("material_ks", new FloatAttribute(0.5f))
		->Set("material_shininess", new IntAttribute(30));

	// Draw buildings
	for (auto b : map->buildings) {
		prop->Set("material_kd", new FloatAttribute(1.0f))
			->Set("spotLight", new IntAttribute(1))
			->Set("positions", new Vec3ArrayAttribute(b->positions, b->numSpotLights))
			->Set("coneAngles", new FloatArrayAttribute(b->coneAngles, b->numSpotLights))
			->Set("coneDirs", new Vec3ArrayAttribute(b->coneDirs, b->numSpotLights))
			->Set("colors", new Vec3ArrayAttribute(b->colors, b->numSpotLights));
		b->Draw(shaders["Lighting"], prop, mapTextures["window"]);
	}

	// Draw roads
	for (auto road : map->roads) {
		prop->Set("textureEnable", new IntAttribute(1))
			->Set("Model", new Mat4Attribute(road->GetModelMatrix()))
			->Set("spotLight", new IntAttribute(0))
			->Set("texture", new TextureAttribute(mapTextures["road_lanes"]))
			->Set("material_kd", new FloatAttribute(0.8f));
		road->RenderMesh(shaders["Lighting"], prop);
	}
	
	// Draw cars
	for (auto car : map->cars) {
		prop->Set("textureEnable", new IntAttribute(0))
			->Set("Model", new Mat4Attribute(car->GetModelMatrix()))
			->Set("color", new Vec3Attribute(glm::vec3(0.25f)))
			->Set("material_kd", new FloatAttribute(0.8f));
		car->RenderMesh(shaders["Lighting"], prop);
	}

	// Draw street lights
	for (auto light : map->streetLights) {
		prop->Set("textureEnable", new IntAttribute(0))
			->Set("Model", new Mat4Attribute(light->GetModelMatrix()))
			->Set("material_kd", new FloatAttribute(0.8f));
		light->RenderMesh(shaders["Lighting"], prop);
	}

	// Draw trees
	for (auto tree : map->trees) {
		prop->Set("textureEnable", new IntAttribute(1))
			->Set("Model", new Mat4Attribute(tree->GetModelMatrix()))
			->Set("texture", new TextureAttribute(mapTextures["BroadleafBark"]))
			->Set("material_kd", new FloatAttribute(0.8f));
		tree->RenderMesh(shaders["Lighting"], prop);
	}

	// Draw ground
	prop->Set("textureEnable", new IntAttribute(1))
		->Set("Model", new Mat4Attribute(ground->GetModelMatrix()))
		->Set("texture", new TextureAttribute(mapTextures["snow"]))
		->Set("material_kd", new FloatAttribute(0.8f)) 
		->Set("material_ks", new FloatAttribute(.1f))
		->Set("material_shininess", new IntAttribute(5));
	ground->RenderMesh(shaders["Lighting"], prop);
}

void Homework::FrameEnd()
{
	//DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Homework::OnInputUpdate(float deltaTime, int mods)
{
	float speed = 3.0f;

	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
		if (mods == GLFW_MOD_SHIFT) {
			speed *= 6;
		}
		if (window->KeyHold(GLFW_KEY_W)) {
			camera->TranslateForward(speed * deltaTime);
		}
		if (window->KeyHold(GLFW_KEY_S)) {
			camera->TranslateForward(-speed * deltaTime);
		}
		if (window->KeyHold(GLFW_KEY_A)) {
			camera->TranslateRight(-speed * deltaTime);
		}
		if (window->KeyHold(GLFW_KEY_D)) {
			camera->TranslateRight(speed * deltaTime);
		}
	}
}

void Homework::OnKeyPress(int key, int mods)
{
	// add key press event
}

void Homework::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Homework::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;
		camera->RotateFirstPerson_OY(-deltaX * sensivityOX);
		camera->RotateFirstPerson_OX(-deltaY * sensivityOY);

		window->DisablePointer();
		window->CenterPointer();
	}
}

void Homework::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Homework::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Homework::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Homework::OnWindowResize(int width, int height)
{
}
