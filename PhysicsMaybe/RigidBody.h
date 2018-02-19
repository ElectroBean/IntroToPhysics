#pragma once
#include "PhysicsObject.h"
#include <Gizmos.h>

class RigidBody :
	public PhysicsObject
{
public:
	RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, bool kinematic, float linearDrag, float angularDrag, float elasticity);
	~RigidBody();
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug();
	void applyForce(glm::vec2 force, glm::vec2 pos);
	//void applyForceToActor(RigidBody* actor2, glm::vec2 force);
	virtual bool checkCollision(PhysicsObject* pOther) = 0;
	glm::vec2 getPosition() { return m_position; }
	float getRotation() { return m_rotation; }
	glm::vec2 getVelocity() { return m_velocity; }
	float getMass() { return m_mass; }
	void changeMass(float massChange);
	void ResolveCollision(RigidBody * actor2, glm::vec2 contact, glm::vec2* collisionNormal = nullptr);
	void setKinematic(bool aInput) { isKinematic = aInput; }
	void setVelocity(glm::vec2 velocity) { m_velocity = velocity; }
	float getLinearDrag() { return m_linearDrag; }
	void setLinearDrag(float drag) { m_linearDrag = drag; }
	float getAngularDrag() { return m_angularDrag; }
	void setAngularDrag(float drag) { m_angularDrag = drag; }
	float getElasticity() { return m_elasticity; }
	void setPosition(glm::vec2 position) { m_position = position; }
	float getMoment() { return m_moment; }

protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_mass;
	float m_rotation; //2D so we only need a single float to represent our rotation
	float m_angularVelocity;
	float m_moment; //how much it is rotated in radians
	bool isKinematic;

	float m_elasticity;

	float m_linearDrag;
	float m_angularDrag;
};
