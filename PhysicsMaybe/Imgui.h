#pragma once

#include <glm\ext.hpp>

class GUI
{
public:
	GUI();
	~GUI();

	void update(float* physTickRate);

private:
	glm::vec4 m_clearColour;
};

