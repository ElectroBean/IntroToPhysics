#pragma once
#include "RigidBody.h"

class Sphere :
	public RigidBody
{
public:
	Sphere(glm::vec2 position, glm::vec2 velocity,
		float mass, bool kinematic, float radius, float linearDrag, float angularDrag, float elasticity, glm::vec4 colour);
	~Sphere();
	virtual void makeGizmo();
	virtual bool checkCollision(PhysicsObject* pOther);
	Sphere(glm::vec2 position, float inclination, float speed, float mass, bool kinematic, float radius, float linearDrag, float angularDrag, float elasticity, glm::vec4 colour);
	float getRadius() { return m_radius; }
	glm::vec4 getColour() { return m_colour; }
protected:
	float m_radius;
	glm::vec4 m_colour;

};

