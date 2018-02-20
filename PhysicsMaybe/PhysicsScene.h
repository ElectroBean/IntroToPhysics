#pragma once
#include <glm/ext.hpp>
#include <vector>
#include "PhysicsObject.h"

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void addActor(PhysicsObject* actor);
	void removeActor(PhysicsObject* actor);
	void update(float dt);
	void debugScene();
	void updateGizmos();
	void setGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	glm::vec2 getGravity() const { return m_gravity; }
	void setTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float getTimeStep() const { return m_timeStep; }
	glm::vec2* getGravityP() {
		return &m_gravity;
	}

	void checkForCollision();

	void DestroyAll();

	static bool plane2Plane(PhysicsObject*, PhysicsObject*);
	static bool plane2Sphere(PhysicsObject*, PhysicsObject*);
	static bool plane2Box(PhysicsObject*, PhysicsObject*);
	static bool plane2aabb(PhysicsObject *, PhysicsObject *);
	static bool sphere2Plane(PhysicsObject*, PhysicsObject*);
	static bool sphere2Sphere(PhysicsObject*, PhysicsObject*);
	static bool sphere2Box(PhysicsObject*, PhysicsObject*);
	static bool sphere2aabb(PhysicsObject *, PhysicsObject *);
	static bool box2Plane(PhysicsObject*, PhysicsObject*);
	static bool box2Sphere(PhysicsObject*, PhysicsObject*);
	static bool box2Box(PhysicsObject*, PhysicsObject*);

	static bool box2aabb(PhysicsObject *, PhysicsObject *);
	
	static bool aabb2Plane(PhysicsObject *, PhysicsObject *);
	
	static bool aabb2Sphere(PhysicsObject *, PhysicsObject *);
	
	static bool aabb2Box(PhysicsObject *, PhysicsObject *);
	
	static bool aabb2aabb(PhysicsObject *, PhysicsObject *);

protected:
	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_actors;

	float asdasd[2] = { 0, 0 };
};

