#include "Laborator4.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
#include "Transform3D.h"

using namespace std;

Laborator4::Laborator4()
{
}

Laborator4::~Laborator4()
{
}

void Laborator4::Init()
{
	polygonMode = GL_FILL;

	Mesh* mesh = new Mesh("box");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
	meshes[mesh->GetMeshID()] = mesh;

	// initialize tx, ty and tz (the translation steps)
	translateX = 0;
	translateY = 0;
	translateZ = 0;

	// initialize sx, sy and sz (the scale factors)
	scaleX = 1;
	scaleY = 1;
	scaleZ = 1;
	
	// initialize angularSteps
	angularStepOX = 0;
	angularStepOY = 0;
	angularStepOZ = 0;
}

void Laborator4::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

double rotAngle = 0;

void Laborator4::Update(float deltaTimeSeconds)
{
	rotAngle += 1 * deltaTimeSeconds;

	glLineWidth(3);
	glPointSize(5);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-2.5f, 0.5f,-1.5f);
	modelMatrix *= Transform3D::Translate(translateX, translateY, translateZ);
	glm::mat4 SunPos = modelMatrix;
	modelMatrix *= Transform3D::RotateOX(rotAngle) * Transform3D::RotateOY(rotAngle);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	modelMatrix = glm::mat4(1);
	modelMatrix *= SunPos;
	modelMatrix *= Transform3D::RotateOY(rotAngle) * Transform3D::Translate(10, 0, 0);
	glm::mat4 earthPos = modelMatrix;
	modelMatrix *= Transform3D::Scale(scaleX, scaleY, scaleZ);
	RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

	modelMatrix = glm::mat4(1);
	modelMatrix *= earthPos;
	modelMatrix *= Transform3D::RotateOY(2 * rotAngle) * Transform3D::Translate(3, 0, 0);
	modelMatrix *= Transform3D::RotateOX(angularStepOX);
	modelMatrix *= Transform3D::RotateOY(angularStepOY);
	modelMatrix *= Transform3D::RotateOZ(angularStepOZ);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
}

void Laborator4::FrameEnd()
{
	DrawCoordinatSystem();
}

void Laborator4::OnInputUpdate(float deltaTime, int mods)
{
	// cu tastele W A S D R F sa deplaseze primul cub in scena
	double acceleration = 1;
	if (window->KeyHold(GLFW_KEY_W)) {
		translateZ += acceleration * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_S)) {
		translateZ -= acceleration * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_A)) {
		translateX += acceleration * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_D)) {
		translateX -= acceleration * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_R)) {
		translateY += acceleration * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_F)) {
		translateY -= acceleration * deltaTime;
	}

	// cu tastele 1 si 2 sa scaleze al doilea cub (sa se mareasca si sa se micsoreze) fata de centrul propriu
	if (window->KeyHold(GLFW_KEY_1)) {
		scaleX += acceleration * deltaTime;
		scaleY += acceleration * deltaTime;
		scaleZ += acceleration * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_2)) {
		scaleX -= acceleration * deltaTime;
		scaleY -= acceleration * deltaTime;
		scaleZ -= acceleration * deltaTime;
	}

	// cu tastele 3,4,5,6,7,8 sa roteasca al treilea cub fata de axele globale OX, OY, OZ
	if (window->KeyHold(GLFW_KEY_3)) {
		angularStepOX += acceleration * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_4)) {
		angularStepOX -= acceleration * deltaTime;
	} 
	if (window->KeyHold(GLFW_KEY_5)) {
		angularStepOY += acceleration * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_6)) {
		angularStepOY -= acceleration * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_7)) {
		angularStepOZ += acceleration * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_8)) {
		angularStepOZ -= acceleration * deltaTime;
	}
}

void Laborator4::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_SPACE)
	{
		switch (polygonMode)
		{
		case GL_POINT:
			polygonMode = GL_FILL;
			break;
		case GL_LINE:
			polygonMode = GL_POINT;
			break;
		default:
			polygonMode = GL_LINE;
			break;
		}
	}
}

void Laborator4::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator4::OnWindowResize(int width, int height)
{
}
