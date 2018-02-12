#pragma once
#include "PhysicsObject.h"
#include <Gizmos.h>

class RigidBody :
	public PhysicsObject
{
public:
	RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, bool kinematic);
	~RigidBody();
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug();
	void applyForce(glm::vec2 force);
	void applyForceToActor(RigidBody* actor2, glm::vec2 force);
	virtual bool checkCollision(PhysicsObject* pOther) = 0;
	glm::vec2 getPosition() { return m_position; }
	float getRotation() { return m_rotation; }
	glm::vec2 getVelocity() { return m_velocity; }
	float getMass() { return m_mass; }
	void changeMass(float massChange);
	void ResolveCollision(RigidBody * actor2);
	void setKinematic(bool aInput) { isKinematic = aInput; }

protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_mass;
	float m_rotation; //2D so we only need a single float to represent our rotation
	bool isKinematic;
};
