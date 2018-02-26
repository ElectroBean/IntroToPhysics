#pragma once
#include "PhysicsObject3D.h"
#include <glm\ext.hpp>
#include <Gizmos.h>

class RigidBody3D :
	public PhysicsObject3D
{
public:
	RigidBody3D(ShapeType shapeID, glm::vec3 position, glm::vec3 velocity, glm::vec3 rotation, float mass, bool kinematic, float linearDrag, float angularDrag, float elasticity);
	~RigidBody3D();
	virtual void fixedUpdate(glm::vec3 gravity, float timeStep);
	virtual void debug();
	void applyForce(glm::vec3 force, glm::vec3 pos);
	//void applyForceToActor(RigidBody* actor2, glm::vec2 force);
	virtual bool checkCollision(PhysicsObject3D* pOther) = 0;
	glm::vec3 getPosition() { return m_position; }
	glm::vec3 getRotation() { return m_rotation; }
	glm::vec3 getVelocity() { return m_velocity; }
	float getMass() { return m_mass; }
	void changeMass(float massChange);
	void ResolveCollision(RigidBody3D * actor2, glm::vec3 contact, glm::vec3* collisionNormal = nullptr);
	void setKinematic(bool aInput) { isKinematic = aInput; }
	void setVelocity(glm::vec3 velocity) { m_velocity = velocity; }
	float getLinearDrag() { return m_linearDrag; }
	void setLinearDrag(float drag) { m_linearDrag = drag; }
	float getAngularDrag() { return m_angularDrag; }
	void setAngularDrag(float drag) { m_angularDrag = drag; }
	float getElasticity() { return m_elasticity; }
	void setPosition(glm::vec3 position) { m_position = position; }
	float getMoment() { return m_moment; }
	bool getIsKinematic() { return isKinematic; }

protected:
	glm::vec3 m_position;
	glm::vec3 m_velocity;
	float m_mass;
	glm::vec3 m_rotation; //2D so we only need a single float to represent our rotation
	glm::vec3 m_angularVelocity;
	float m_moment; //how much it is rotated in radians
	bool isKinematic;

	float m_elasticity;

	float m_linearDrag;
	float m_angularDrag;
};

