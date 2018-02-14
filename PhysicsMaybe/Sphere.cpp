#include "Sphere.h"
#include "RigidBody.h"
#include <Renderer2D.h>
#include <Gizmos.h>

Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, bool kinematic, float radius, float linearDrag, float angularDrag, float elasticity, glm::vec4 colour)
	: RigidBody(SPHERE, position, velocity, 0, mass, kinematic, 0, 0, 0)
{
	m_radius = radius;
	m_colour = colour;
	this->m_linearDrag = linearDrag;
	this->m_angularDrag = angularDrag;
	this->m_elasticity = elasticity;
}

Sphere::~Sphere()
{
}

void Sphere::makeGizmo()
{
	aie::Gizmos::add2DCircle(m_position, m_radius, 16, m_colour);
}

bool Sphere::checkCollision(PhysicsObject * pOther)
{
	//dynamic cast
	Sphere* other = dynamic_cast<Sphere*>(pOther);

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

Sphere::Sphere(glm::vec2 position, float inclination, 
	float speed, float mass, bool kinematic, float radius, float linearDrag, float angularDrag, float elasticity, glm::vec4 colour) :

			Sphere(position, glm::vec2(sin(inclination), cos(inclination)) * speed, mass, kinematic, radius, linearDrag, angularDrag, elasticity, colour)
{

}
