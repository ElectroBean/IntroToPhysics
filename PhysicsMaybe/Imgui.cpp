#include "Imgui.h"
#include <glm\ext.hpp>
#include <imgui.h>
#include <gl_core_4_4.h>

GUI::GUI()
{
	m_clearColour = glm::vec4(1, 1, 1, 1);
}


GUI::~GUI()
{
}

void GUI::update(float* exhaustTimer)
{
	ImGui::Begin("My options");
	glClearColor(m_clearColour.r, m_clearColour.g, m_clearColour.b, 1);
	ImGui::ColorEdit3("clear colour", glm::value_ptr(m_clearColour));
	ImGui::SliderFloat("Physics Tick Rate", exhaustTimer, 0.01f, 0.1f);
	ImGui::End();
}


