#include "AABB.h"


AABB::AABB(glm::vec2 position, glm::vec2 extents, glm::vec2 velocity, float mass, bool kinematic, float linearDrag, float angularDrag, float elasticity, glm::vec4 colour)
	: RigidBody(aabb, position, velocity, 0, mass, kinematic, 0, 0, 0)
{
	this->extents = extents;
	this->colour = colour;
	this->m_linearDrag = linearDrag;
	this->m_angularDrag = angularDrag;
	this->m_elasticity = elasticity;
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
