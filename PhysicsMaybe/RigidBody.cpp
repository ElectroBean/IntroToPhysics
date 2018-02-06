#include "RigidBody.h"

RigidBody::RigidBody(ShapeType shapeID, glm::vec2 position,
	glm::vec2 velocity, float rotation, float mass, bool kinematic) : PhysicsObject(shapeID), m_position(position),
	m_velocity(velocity), m_rotation(rotation), m_mass(mass), isKinematic(kinematic)
{
	m_position = position;
	m_velocity = velocity;
	m_rotation = rotation;
	m_mass = mass;
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


		static float aspectRatio = 16 / 9.f;
		if (m_position.x > 100)
		{
			m_position.x = -100;
		}
		if (m_position.x < -100)
		{
			m_position.x = 100;
		}
		if (m_position.y > 100 / aspectRatio)
		{
			m_position.y = -100 / aspectRatio;
		}
		if (m_position.y < -100 / aspectRatio)
		{
			m_position.y = 100 / aspectRatio;
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


