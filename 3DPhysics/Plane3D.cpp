#include "Plane3D.h"
#include <Gizmos.h>


Plane3D::Plane3D() : PhysicsObject3D(ShapeType::PLANE)
{
	m_distanceToOrigin = 0;
	m_normal = glm::vec3(0, 1, 0);
}

Plane3D::Plane3D(glm::vec3 normal, float distance) : PhysicsObject3D(ShapeType::PLANE)
{
	m_distanceToOrigin = distance;
	m_normal = glm::normalize(normal);
}


Plane3D::~Plane3D()
{
}

void Plane3D::fixedUpdate(glm::vec3 gravity, float timeStep)
{
}

void Plane3D::makeGizmo()
{
	glm::vec3 planeNorm = glm::normalize(m_normal);
	glm::mat4 rotation = glm::lookAt(glm::vec3(0), glm::vec3(0), planeNorm);
	rotation[3] = glm::vec4(planeNorm * m_distanceToOrigin, 1);

	aie::Gizmos::addDisk(glm::vec3(0), 5, 4, glm::vec4(1, 1, 0, 0.5), &rotation);
}

void Plane3D::resetPosition()
{
	m_distanceToOrigin = 0;
	m_normal = glm::vec3(0, 1, 0);
}

void Plane3D::ResolveCollision(RigidBody3D * actor2, glm::vec3 contact)
{
	glm::vec3 normal = m_normal;
	glm::vec3 relativeVelocity = actor2->getVelocity();
	float elasticity = 1;
	float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) / (1 / actor2->getMass());
	glm::vec3 force = normal * j;
	actor2->applyForce(force, contact - actor2->getPosition());
}
