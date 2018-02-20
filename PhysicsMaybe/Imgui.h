#pragma once
#include <glm\ext.hpp>
#include "PhysicsScene.h"

class GUI
{
public:
	GUI();
	~GUI();

	void update(float* physTickRate, PhysicsScene* PhysicsScene);

private:
	glm::vec4 m_clearColour;
	float gravity[2] = {0, -9.8f};
	float tickrate = 0.015f;
};

