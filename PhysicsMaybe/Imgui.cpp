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

void GUI::update(float* exhaustTimer, glm::vec2* gravity)
{
	ImGui::Begin("My options");
	float asdasd[2] = { gravity->x, gravity->y };

	if (ImGui::Button("Randomize Colour?"))
	{
		float r = rand() % 2 + 0.1f;
		float g = rand() % 2 + 0.1f;
		float b = rand() % 2 + 0.1f;

		m_clearColour = glm::vec4(r, g, b, 1);
	}
	glClearColor(m_clearColour.r, m_clearColour.g, m_clearColour.b, 1);
	ImGui::ColorEdit3("clear colour", glm::value_ptr(m_clearColour));
	//ImGui::SliderFloat("Rocket Exhaust Timer", exhaustTimer, 0.01f, 0.1f);
	ImGui::End();
}


