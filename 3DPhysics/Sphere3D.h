#pragma once
#include "RigidBody3D.h"
class Sphere3D :
	public RigidBody3D
{
public:
	Sphere3D(glm::vec3 position, glm::vec3 velocity,
		float mass, bool kinematic, float radius, float linearDrag, float angularDrag, float elasticity, glm::vec4 colour);
	~Sphere3D();
	virtual void makeGizmo();
	virtual bool checkCollision(PhysicsObject3D* pOther);
	float getRadius() { return m_radius; }
	glm::vec4 getColour() { return m_colour; }
protected:
	float m_radius;
	glm::vec4 m_colour;
};

