#pragma once
#include "RigidBody.h"
class AABB :
	public RigidBody
{
public:
	AABB(glm::vec2 position, glm::vec2 extents, glm::vec2 velocity, float mass, bool kinematic, glm::vec4 colour);
	~AABB();

	void makeGizmo();

	bool checkCollision(PhysicsObject * pOther);
	glm::vec2 getExtents() { return extents; }
	glm::vec2 getMax() { return this->getPosition() + this->getExtents(); }
	glm::vec2 getMin() { return this->getPosition() - this->getExtents(); }

private: 
	glm::vec2 extents;
	glm::vec4 colour;
};

