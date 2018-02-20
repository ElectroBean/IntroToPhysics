#include "Imgui.h"
#include <glm\ext.hpp>
#include <imgui.h>
#include <gl_core_4_4.h>

GUI::GUI()
{
	m_clearColour = glm::vec4(1, 1, 1, 1);
	srand((unsigned)time(0));
}


GUI::~GUI()
{
}

void GUI::update(float* physTickRate, PhysicsScene* PhysicsScene)
{
	ImGui::Begin("My options");

	// Physics Gravity
	ImGui::SliderFloat2("gravity", gravity, -9.8f, 9.8f);
	glm::vec2 grav = glm::vec2(gravity[0], gravity[1]);
	PhysicsScene->setGravity(grav);

	// PhysicsTickRate
	ImGui::SliderFloat("tick rate", &tickrate, 0.015f, 1.0f);
	PhysicsScene->setTimeStep(tickrate);

	//Destroy all ojects

	//needs work
	//doesnt destroy all objects, just a couple
	/*
	if (ImGui::Button("Destroy all"))
	{
		PhysicsScene->DestroyAll();
	}
	*/

	ImGui::End();
}


