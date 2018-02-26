#pragma once
#include <glm\ext.hpp>

using namespace glm;

class Camera3D
{
public:
	Camera3D();
	~Camera3D();

	void update();

protected:
	mat4 view;
	mat4 projection;

	float currentTime;
	float deltaTime;
	float previousTime;
};

