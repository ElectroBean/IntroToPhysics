#include "Camera3D.h"



Camera3D::Camera3D()
{
}


Camera3D::~Camera3D()
{
}

void Camera3D::update()
{
	view = lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	projection = perspective(pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

	
}
