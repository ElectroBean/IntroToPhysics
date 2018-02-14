#include "RigidBody.h"

RigidBody::RigidBody(ShapeType shapeID, glm::vec2 position,
	glm::vec2 velocity, float rotation, float mass, bool kinematic, float linearDrag, float angularDrag, float elasticity) : PhysicsObject(shapeID), m_position(position),
	m_velocity(velocity), m_rotation(rotation), m_mass(mass), isKinematic(kinematic)
{
	m_position = position;
	m_velocity = velocity;
	m_rotation = rotation;
	m_mass = mass;
	m_linearDrag = linearDrag;
	m_angularDrag = angularDrag;
	m_elasticity = elasticity;
}


RigidBody::~RigidBody()
{

}

void RigidBody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	if (!isKinematic)
	{
		applyForce(gravity * m_mass * timeStep);
		m_position += m_velocity * timeStep;

		m_velocity -= m_velocity * m_linearDrag * timeStep;
		
		if (length(m_velocity) < 0.1f) 
		{
			m_velocity = glm::vec2(0, 0); 
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

void RigidBody::debug()
{
}

void RigidBody::applyForce(glm::vec2 force)
{
	this->m_velocity += force / this->m_mass;
}

void RigidBody::applyForceToActor(RigidBody * actor2, glm::vec2 force)
{
	applyForce(-force);
	actor2->applyForce(force);
}

void RigidBody::changeMass(float massChange)
{
	m_mass += massChange;
}

void RigidBody::ResolveCollision(RigidBody* actor2)
{
	glm::vec2 normal = glm::normalize(actor2->getPosition() - m_position); 
	glm::vec2 relativeVelocity = actor2->getVelocity() - m_velocity; 
	float elasticity = (m_elasticity + actor2->getElasticity()) / 2.0f;
	float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) / glm::dot(normal, normal * ((1 / m_mass) + (1 / actor2->getMass()))); 
	glm::vec2 force = normal * j; 
	applyForceToActor(actor2, force);
}


