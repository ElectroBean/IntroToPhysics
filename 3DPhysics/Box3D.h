#pragma once
#include "RigidBody3D.h"
class Box3D :
	public RigidBody3D
{
public:
	Box3D(glm::vec3 position, glm::vec3 extents, glm::vec3 velocity, float mass, bool kinematic, float linearDrag, float angularDrag, float elasticity, glm::vec4 colour);
	~Box3D();

	void makeGizmo();

	bool checkCollision(PhysicsObject3D * pOther);
	const glm::vec3 getExtents() { return extents; }
	glm::vec3 getMax() { return this->getPosition() + this->getExtents(); }
	glm::vec3 getMin() { return this->getPosition() - this->getExtents(); }

	glm::vec3 getLocalX() { return m_localX; }
	glm::vec3 getLocalY() { return m_localY; }

	float getHeight() { return extents.y * 2; }
	float getWidth() { return extents.x * 2; }

	void fixedUpdate(glm::vec3 gravity, float timeStep);

	glm::vec2 getCenter() { return this->getPosition(); }


	bool checkBoxCorners(Box3D & box, glm::vec3 & contact, int & numContacts, float & pen, glm::vec3 & edgeNormal, glm::vec3& contactForce);

private:

	glm::vec3 extents;
	glm::vec4 colour;

	glm::vec3 m_localX = glm::vec3(0, 0, 0);
	glm::vec3 m_localY = glm::vec3(0, 0, 0);
};

