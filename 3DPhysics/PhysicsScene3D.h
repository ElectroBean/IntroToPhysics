#pragma once
#include "PhysicsObject3D.h"
#include <vector>
#include <glm\ext.hpp>

class PhysicsScene3D
{
public:
	PhysicsScene3D();
	~PhysicsScene3D();

	void addActor(PhysicsObject3D* actor);
	void removeActor(PhysicsObject3D* actor);
	void update(float dt);
	void debugScene();
	void updateGizmos();
	void setGravity(const glm::vec3 gravity) { m_gravity = gravity; }
	glm::vec3 getGravity() const { return m_gravity; }
	void setTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float getTimeStep() const { return m_timeStep; }
	glm::vec3* getGravityP() {
		return &m_gravity;
	}

	void checkForCollision();

	void DestroyAll();

	static bool plane2Plane(PhysicsObject3D*, PhysicsObject3D*);
	static bool plane2Sphere(PhysicsObject3D*, PhysicsObject3D*);
	static bool plane2Box(PhysicsObject3D*, PhysicsObject3D*);
	static bool plane2aabb(PhysicsObject3D *, PhysicsObject3D *);
	static bool sphere2Plane(PhysicsObject3D*, PhysicsObject3D*);
	static bool sphere2Sphere(PhysicsObject3D*, PhysicsObject3D*);
	static bool sphere2Box(PhysicsObject3D*, PhysicsObject3D*);
	static bool sphere2aabb(PhysicsObject3D *, PhysicsObject3D *);
	static bool box2Plane(PhysicsObject3D*, PhysicsObject3D*);
	static bool box2Sphere(PhysicsObject3D*, PhysicsObject3D*);
	static bool box2Box(PhysicsObject3D*, PhysicsObject3D*);

	static bool box2aabb(PhysicsObject3D *, PhysicsObject3D *);

	static bool aabb2Plane(PhysicsObject3D *, PhysicsObject3D *);

	static bool aabb2Sphere(PhysicsObject3D *, PhysicsObject3D *);

	static bool aabb2Box(PhysicsObject3D *, PhysicsObject3D *);

	static bool aabb2aabb(PhysicsObject3D *, PhysicsObject3D *);

protected:
	glm::vec3 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject3D*> m_actors;
};

