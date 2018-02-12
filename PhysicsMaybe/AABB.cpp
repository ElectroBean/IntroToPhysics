#include "AABB.h"


AABB::AABB(glm::vec2 position, glm::vec2 extents, glm::vec2 velocity, float mass, bool kinematic, glm::vec4 colour) 
	: RigidBody(aabb, position, velocity, 0, mass, kinematic)
{
	this->extents = extents;
	this->colour = colour;
}

AABB::~AABB()
{

}

void AABB::makeGizmo()
{
	aie::Gizmos::add2DAABBFilled(this->m_position, this->extents, this->colour);
}

bool AABB::checkCollision(PhysicsObject * pOther)
{
	

	return false;
}
