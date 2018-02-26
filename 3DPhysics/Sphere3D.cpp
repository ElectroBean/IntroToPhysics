#include "Sphere3D.h"

Sphere3D::Sphere3D(glm::vec3 position, glm::vec3 velocity, float mass, bool kinematic, float radius, float linearDrag, float angularDrag, float elasticity, glm::vec4 colour)
	: RigidBody3D(SPHERE, position, velocity, glm::vec3(), mass, kinematic, 0, 0, 0)
{
	m_radius = radius;
	m_colour = colour;
	this->m_linearDrag = linearDrag;
	this->m_angularDrag = angularDrag;
	this->m_elasticity = elasticity;
	this->m_moment = 0.5f * m_mass * m_radius * m_radius;
	m_angularVelocity = glm::vec3();
}

Sphere3D::~Sphere3D()
{

}

void Sphere3D::makeGizmo()
{
	aie::Gizmos::addSphere(m_position, m_radius, 10, 10, m_colour);
}

bool Sphere3D::checkCollision(PhysicsObject3D * pOther)
{
	//dynamic cast
	Sphere3D* other = dynamic_cast<Sphere3D*>(pOther);

	if (other)
	{
		if ((other->m_radius + this->m_radius) >
			glm::distance(other->m_position, this->m_position))
		{
			return true;
		}
		else return false;
	}

	return false;
}


