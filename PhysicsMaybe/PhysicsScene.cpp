#include "PhysicsScene.h"
#include "RigidBody.h"
#include <list>
#include <iostream>
#include "Sphere.h"
#include "Plane.h"
#include "AABB.h"
#include "Imgui.h"
#include <glm\ext.hpp>
#include <imgui.h>
#include <gl_core_4_4.h>

PhysicsScene::PhysicsScene()
{
	setTimeStep(0.01f);
	setGravity(glm::vec2(0, 0));
}


PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_actors)
	{
		delete pActor;
	}

}

void PhysicsScene::addActor(PhysicsObject * actor)
{
	m_actors.push_back(actor);
}

void PhysicsScene::removeActor(PhysicsObject * actor)
{
	for (auto iter = m_actors.begin(); iter != m_actors.end(); iter++)
	{
		if (*iter == actor)
		{
			m_actors.erase(iter);
			break;
		}
	}
}

void PhysicsScene::update(float dt)
{
	static std::list<PhysicsObject*> dirty;

	ImGui::Begin("Physics Options");
	ImGui::SliderFloat2("Gravity", asdasd, -9.8f, 9.8f);
	ImGui::SliderFloat("Physics Time Step", &m_timeStep, 0.001f, 1.0f);
	m_gravity = glm::vec2(asdasd[0], asdasd[1]);
	ImGui::End();

	// update physics at a fixed time step
	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;
	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->fixedUpdate(m_gravity, m_timeStep);
		}
		accumulatedTime -= m_timeStep;


		//check collisions
		checkForCollision();
		//for (auto pActor : m_actors)
		//{
		//	for (auto pOther : m_actors)
		//	{
		//		if (pActor == pOther)
		//		{
		//			continue;
		//		}
		//
		//		if (std::find(dirty.begin(), dirty.end(), pActor) != dirty.end() &&
		//			std::find(dirty.begin(), dirty.end(), pOther) != dirty.end())
		//			continue;
		//
		//		RigidBody* pRigid = dynamic_cast<RigidBody*>(pActor);
		//		if (pRigid->checkCollision(pOther) == true) {
		//			pRigid->applyForceToActor(
		//				dynamic_cast<RigidBody*>(pOther),
		//				pRigid->getVelocity() * pRigid->getMass());
		//			dirty.push_back(pRigid);
		//			dirty.push_back(pOther);
		//		}
		//	}
		//}
	}
}

void PhysicsScene::debugScene()
{
	int count = 0;
	for (auto pActor : m_actors) {
		std::cout << count << " : ";
		pActor->debug();
		count++;
	}

}

void PhysicsScene::updateGizmos()
{
	for (auto pActor : m_actors)
	{
		pActor->makeGizmo();
	}
}

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
	PhysicsScene::plane2Plane, PhysicsScene::plane2Sphere, PhysicsScene::plane2Box, PhysicsScene::plane2aabb,
	PhysicsScene::sphere2Plane, PhysicsScene::sphere2Sphere, PhysicsScene::plane2Box, PhysicsScene::sphere2aabb,
	PhysicsScene::box2Plane, PhysicsScene::box2Sphere, PhysicsScene::box2Box, PhysicsScene::box2aabb,
	PhysicsScene::aabb2Plane, PhysicsScene::aabb2Sphere, PhysicsScene::aabb2Box, PhysicsScene::aabb2aabb,
};

void PhysicsScene::checkForCollision()
{
	int actorCount = m_actors.size();

	//need to check for collisions against all object except this one
	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsObject* object1 = m_actors[outer];
			PhysicsObject* object2 = m_actors[inner];
			int shapeId1 = object1->getShapeID();
			int shapeId2 = object2->getShapeID();
			// using function pointers 
			int functionIdx = (shapeId1 * SHAPE_COUNT) + shapeId2;
			fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
			if (collisionFunctionPtr != nullptr)
			{
				// did a collision occur? 
				collisionFunctionPtr(object1, object2);
			}
		}
	}
}

bool PhysicsScene::plane2Plane(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::plane2Sphere(PhysicsObject *obj1, PhysicsObject *obj2)
{
	return sphere2Plane(obj2, obj1);
}

bool PhysicsScene::plane2Box(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::plane2aabb(PhysicsObject *obj1, PhysicsObject *obj2)
{
	Plane *plane = dynamic_cast<Plane*>(obj1);
	AABB *aabb = dynamic_cast<AABB*>(obj2);

	//get vertices of aabb
	glm::vec2 bottomLeft = glm::vec2(aabb->getPosition().x - aabb->getExtents().x, aabb->getPosition().y - aabb->getExtents().y);
	glm::vec2 bottomRight = glm::vec2(aabb->getPosition().x + aabb->getExtents().x, aabb->getPosition().y - aabb->getExtents().y);
	glm::vec2 topLeft = glm::vec2(aabb->getPosition().x - aabb->getExtents().x, aabb->getPosition().y + aabb->getExtents().y);
	glm::vec2 topRight = glm::vec2(aabb->getPosition().x + aabb->getExtents().x, aabb->getPosition().y + aabb->getExtents().y);
	
	
	if (glm::dot(bottomLeft, plane->getNormal()) - plane->getDistance() < 0)
	{
		plane->ResolveCollision(aabb);
		//aabb->setVelocity(glm::vec2(0, 0));
	}
	else if (glm::dot(bottomRight, plane->getNormal()) - plane->getDistance() < 0)
	{
		plane->ResolveCollision(aabb);
		//aabb->setVelocity(glm::vec2(0, 0));
	}
	else if (glm::dot(topLeft, plane->getNormal()) - plane->getDistance() < 0)
	{
		plane->ResolveCollision(aabb);
		//aabb->setVelocity(glm::vec2(0, 0));
	}
	else if (glm::dot(topRight, plane->getNormal()) - plane->getDistance() < 0)
	{
		plane->ResolveCollision(aabb);
		//aabb->setVelocity(glm::vec2(0, 0));
	}
	
	return false;
}

bool PhysicsScene::sphere2Plane(PhysicsObject *obj1, PhysicsObject *obj2)
{
	Sphere *sphere = dynamic_cast<Sphere*>(obj1);
	Plane *plane = dynamic_cast<Plane*>(obj2);
	//if we are successful then test for collision 
	if (sphere != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->getNormal();
		float sphereToPlane = glm::dot(sphere->getPosition(), plane->getNormal()) - plane->getDistance();
		// if we are behind plane then we flip the normal 
		if (sphereToPlane < 0)
		{
			collisionNormal *= -1; sphereToPlane *= -1;
		}
		float intersection = sphere->getRadius() - sphereToPlane;
		if (intersection > 0)
		{
			//set sphere velocity to zero here 
			plane->ResolveCollision(sphere);
			//sphere->setVelocity(glm::vec2(0, 0));
			return true;
		}
	}

	return false;
}

bool PhysicsScene::sphere2Sphere(PhysicsObject *obj1, PhysicsObject *obj2)
{
	Sphere* sphere1 = dynamic_cast<Sphere*>(obj1);
	Sphere* sphere2 = dynamic_cast<Sphere*>(obj2);

	if (sphere1 != nullptr && sphere2 != nullptr)
	{
		glm::vec2 direction = sphere1->getPosition() - sphere2->getPosition();
		float mag = glm::length(direction);

		float radiiSum = sphere1->getRadius() + sphere2->getRadius();

		if (mag < radiiSum)
		{
			//apply forces to actors


			sphere1->ResolveCollision(sphere2);
			return true;
		}
	}
	return false;
}

bool PhysicsScene::sphere2Box(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::sphere2aabb(PhysicsObject *obj1, PhysicsObject *obj2)
{
	return aabb2Sphere(obj2, obj1);
}

bool PhysicsScene::box2Plane(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::box2Sphere(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::box2Box(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::box2aabb(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::aabb2Plane(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::aabb2Sphere(PhysicsObject *obj1, PhysicsObject *obj2)
{
	AABB *aabb = dynamic_cast<AABB*>(obj1);
	Sphere *sphere = dynamic_cast<Sphere*>(obj2);
	
	//if we are successful then test for collision 
	if (aabb != nullptr && sphere != nullptr)
	{
		glm::vec2 A = glm::clamp(sphere->getPosition(), aabb->getMin(), aabb->getMax());
		glm::vec2 V = A - sphere->getPosition();

		if (glm::length(V) <= sphere->getRadius())
		{
			glm::vec2 normal = glm::normalize(V);
			sphere->setPosition(sphere->getPosition() - normal);
			aabb->ResolveCollision(sphere);
		}
	}
	return false;
}

bool PhysicsScene::aabb2Box(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::aabb2aabb(PhysicsObject *obj1, PhysicsObject *obj2)
{
	AABB *aabb1 = dynamic_cast<AABB*>(obj1);
	AABB *aabb2 = dynamic_cast<AABB*>(obj2);

	//if we are successful then test for collision 
	if (aabb1 != nullptr && aabb2 != nullptr)
	{
		//check collision
		glm::vec2 min1 = glm::vec2((aabb1->getPosition().x - aabb1->getExtents().x), (aabb1->getPosition().y - aabb1->getExtents().y));
		glm::vec2 max1 = glm::vec2((aabb1->getPosition().x + aabb1->getExtents().x), (aabb1->getPosition().y + aabb1->getExtents().y));
		glm::vec2 min2 = glm::vec2((aabb2->getPosition().x - aabb2->getExtents().x), (aabb2->getPosition().y - aabb2->getExtents().y));
		glm::vec2 max2 = glm::vec2((aabb2->getPosition().x + aabb2->getExtents().x), (aabb2->getPosition().y + aabb2->getExtents().y));

		bool collision = (max1.x < min2.x || max2.x < min1.x || max1.y < min2.y || max2.y < min1.y);
		if(!collision)
		{	
			aabb1->ResolveCollision(aabb2);
		}
	}
	return false;
}



