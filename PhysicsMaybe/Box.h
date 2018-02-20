#pragma once
#include "RigidBody.h"
class Box :
	public RigidBody
{
public:
	Box(glm::vec2 position, glm::vec2 extents, glm::vec2 velocity, float mass, bool kinematic, float linearDrag, float angularDrag, float elasticity, glm::vec4 colour);
	~Box();

	void makeGizmo();

	bool checkCollision(PhysicsObject * pOther);
	const glm::vec2 getExtents() { return extents; }
	glm::vec2 getMax() { return this->getPosition() + this->getExtents(); }
	glm::vec2 getMin() { return this->getPosition() - this->getExtents(); }

	glm::vec2 getLocalX() { return m_localX; }
	glm::vec2 getLocalY() { return m_localY; }

	float getHeight() { return extents.y * 2; }
	float getWidth() { return extents.x * 2; }

	void fixedUpdate(glm::vec2 gravity, float timeStep);

	glm::vec2 getCenter() { return this->getPosition(); }


	bool checkBoxCorners(Box & box, glm::vec2 & contact, int & numContacts, float & pen, glm::vec2 & edgeNormal, glm::vec2& contactForce);

private:

	glm::vec2 extents;
	glm::vec4 colour;

	glm::vec2 m_localX = glm::vec2(0, 0);
	glm::vec2 m_localY = glm::vec2(0, 0);
};

