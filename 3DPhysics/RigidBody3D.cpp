#include "RigidBody3D.h"
#include <glm\ext.hpp>


#define angularVel 0.1
RigidBody3D::RigidBody3D(ShapeType shapeID, glm::vec3 position,
	glm::vec3 velocity, glm::vec3 rotation, float mass, bool kinematic, float linearDrag, float angularDrag, float elasticity) :
	PhysicsObject3D(shapeID), m_position(position),m_velocity(velocity), m_rotation(rotation), m_mass(mass), isKinematic(kinematic)
{
	m_position = position;
	m_velocity = velocity;
	m_rotation = rotation;
	m_mass = mass;
	m_linearDrag = linearDrag;
	m_angularDrag = angularDrag;
	m_elasticity = elasticity;
	m_angularVelocity = glm::vec3();
}


RigidBody3D::~RigidBody3D()
{
}

void RigidBody3D::fixedUpdate(glm::vec3 gravity, float timeStep)
{
	if (!isKinematic)
	{
		m_velocity += gravity * timeStep;
		m_position += m_velocity * timeStep;

		m_velocity -= m_velocity * m_linearDrag * timeStep;

		m_rotation += m_angularVelocity * timeStep;
		m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;

		//
		if (length(m_velocity) < 0.1f)
		{
			m_velocity = glm::vec3(0, 0, 0);
		}

		//angular drag min
		//float absolute = (m_angularVelocity);
		if (glm::length(m_angularVelocity) < 0.1f)
		{
			m_angularVelocity = glm::vec3();
		}

		static float aspectRatio = 16 / 9.f;
		if (m_position.x > 100)
		{
			m_velocity.x *= -1.0f;
		}
		if (m_position.x < -100)
		{
			m_velocity.x *= -1.0f;
		}
		if (m_position.y > 100 / aspectRatio)
		{
			m_velocity.y *= -1.0f;
		}
		if (m_position.y < -100 / aspectRatio)
		{
			m_velocity.y *= -1.0f;
		}
	}
}

void RigidBody3D::debug()
{
}

void RigidBody3D::applyForce(glm::vec3 force, glm::vec3 pos)
{
	this->m_velocity += force / this->m_mass;
	m_angularVelocity += ((force.y * pos.x) - (force.x * pos.y)) / (m_moment);
}

void RigidBody3D::changeMass(float massChange)
{
	m_mass += massChange;
}

void RigidBody3D::ResolveCollision(RigidBody3D* actor2, glm::vec3 contact, glm::vec3* collisionNormal)
{
	//glm::vec2 normal = glm::normalize(actor2->getPosition() - m_position); 
	//glm::vec2 relativeVelocity = actor2->getVelocity() - m_velocity; 
	//float elasticity = (m_elasticity + actor2->getElasticity()) / 2.0f;
	//float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) / glm::dot(normal, normal * ((1 / m_mass) + (1 / actor2->getMass()))); 
	//glm::vec2 force = normal * j; 
	//applyForceToActor(actor2, force);

	glm::vec3 normal = collisionNormal ? *collisionNormal : glm::normalize(actor2->m_position - m_position);

	//get the vector perp. to the collision norm
	glm::vec3 perp(normal.y, -normal.x, normal.z);

	//determine the total velocity of the contact points for the two objects, 
	//for both linear and rotational

	//r is the radius from axis to application of force

	float r1 = glm::dot(contact - m_position, -perp);
	float r2 = glm::dot(contact - actor2->m_position, perp);
	glm::vec3 v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;
	glm::vec3 v2 = glm::dot(actor2->m_velocity, normal) + r2 * actor2->m_angularVelocity;

	if (glm::length(v1) > glm::length(v2))
	{
		float mass1 = 1.0f / (1.0f / m_mass + (r1 * r1) / m_moment);
		float mass2 = 1.0f / (1.0f / actor2->m_mass + (r2 * r2) / actor2->m_moment);

		float elasticity = (m_elasticity + actor2->getElasticity()) / 2.0f;

		glm::vec3 force = (1.0f + elasticity) * mass1 * mass2 /
			(mass1 + mass2) * (v1 - v2) * normal;

		applyForce(-force, contact - m_position);
		actor2->applyForce(force, contact - actor2->m_position);
	}
}
