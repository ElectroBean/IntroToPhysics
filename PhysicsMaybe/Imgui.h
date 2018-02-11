#pragma once
#include <glm\ext.hpp>


class GUI
{
public:
	GUI();
	~GUI();

	void update(float* physTickRate, glm::vec2* gravity);

private:
	glm::vec4 m_clearColour;
};

