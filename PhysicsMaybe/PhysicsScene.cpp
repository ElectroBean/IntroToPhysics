#include "PhysicsScene.h"
#include "RigidBody.h"
#include <list>
#include <iostream>
#include "Sphere.h"
#include "Plane.h"
#include "AABB.h"
#include "Box.h"
#include "Imgui.h"
#include <glm\ext.hpp>
#include <imgui.h>
#include <gl_core_4_4.h>

PhysicsScene::PhysicsScene()
{
	setTimeStep(0.01f);
	setGravity(glm::vec2(-9.8f, 0));
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
	PhysicsScene::sphere2Plane, PhysicsScene::sphere2Sphere, PhysicsScene::sphere2Box, PhysicsScene::sphere2aabb,
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
		//plane->ResolveCollision(aabb);
		//aabb->setVelocity(glm::vec2(0, 0));
	}
	else if (glm::dot(bottomRight, plane->getNormal()) - plane->getDistance() < 0)
	{
		//plane->ResolveCollision(aabb);
		//aabb->setVelocity(glm::vec2(0, 0));
	}
	else if (glm::dot(topLeft, plane->getNormal()) - plane->getDistance() < 0)
	{
		//plane->ResolveCollision(aabb);
		//aabb->setVelocity(glm::vec2(0, 0));
	}
	else if (glm::dot(topRight, plane->getNormal()) - plane->getDistance() < 0)
	{
		//plane->ResolveCollision(aabb);
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
			glm::vec2 contact = sphere->getPosition() + (collisionNormal * -sphere->getRadius());
			sphere->setPosition(sphere->getPosition() + plane->getNormal() * (sphere->getRadius() - sphereToPlane));
			plane->ResolveCollision(sphere, contact);
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
			//get direction vector
			//glm::vec2 vd = sphere2->getPosition() - sphere1->getPosition();
			//
			////get the length of vector
			//float magVD = glm::length(vd);
			//
			////get overlap
			//float overlap = radiiSum - mag;
			//
			////normal of vd * (1/2 * overlap)
			//glm::vec2 ov = (glm::normalize(vd) * 0.5f * overlap);
			//
			////set positions of spheres
			//sphere1->setPosition(sphere1->getPosition() + ov);
			//sphere2->setPosition(sphere1->getPosition() - ov);

			//glm::vec2 movementVec = sphere2->getPosition() - sphere1->getPosition();
			//movementVec = glm::normalize(movementVec);
			//float distfromobj = radiiSum - mag;
			//sphere1->setPosition(sphere1->getPosition() - movementVec * distfromobj * 0.5f);
			//sphere2->setPosition(sphere2->getPosition() + movementVec * distfromobj * 0.5f);

			glm::vec2 contactForce = 0.5f * (mag - (sphere1->getRadius() + sphere2->getRadius())) * direction / mag;

			sphere1->setPosition(sphere1->getPosition() + contactForce); 
			sphere2->setPosition(sphere2->getPosition() - contactForce);


			//apply forces to actors
			sphere1->ResolveCollision(sphere2, 0.5f * (sphere1->getPosition() + sphere2->getPosition()));
			return true;
		}
	}
	return false;
}

bool PhysicsScene::sphere2Box(PhysicsObject *obj1, PhysicsObject *obj2)
{
	box2Sphere(obj2, obj1);
	return false;
}

bool PhysicsScene::sphere2aabb(PhysicsObject *obj1, PhysicsObject *obj2)
{
	return aabb2Sphere(obj2, obj1);
}

bool PhysicsScene::box2Plane(PhysicsObject *obj1, PhysicsObject *obj2)
{
	Box *box = dynamic_cast<Box*>(obj1);
	Plane *plane = dynamic_cast<Plane*>(obj2);
	//if we are successful then test for collision 
	if (box != nullptr && plane != nullptr)
	{
		int numContacts = 0;
		glm::vec2 contact(0, 0);
		float contactV = 0;
		float radius = 0.5f * std::fminf(box->getWidth(), box->getHeight());
		// which side is the centre of mass on? 
		glm::vec2 planeOrigin = plane->getNormal() * plane->getDistance();
		float comFromPlane = glm::dot(box->getPosition() - planeOrigin, plane->getNormal());
		// check all four corners to see if we've hit the plane 
		for (float x = -box->getExtents().x; x < box->getWidth(); x += box->getWidth())
		{
			for (float y = -box->getExtents().y; y < box->getHeight(); y += box->getHeight())
			{
				// get the position of the corner in world space
				glm::vec2 p = box->getPosition() + x * box->getLocalX() + y * box->getLocalY();
				float distFromPlane = glm::dot(p - planeOrigin, plane->getNormal());
				// this is the total velocity of the point 
				float velocityIntoPlane = glm::dot(box->getVelocity() + box->getRotation() * (-y * box->getLocalX() + x * box->getLocalY()), plane->getNormal());
				// if this corner is on the opposite side from the COM, 
				// and moving further in, we need to resolve the collision
				if ((distFromPlane > 0 && comFromPlane < 0 && velocityIntoPlane > 0) || (distFromPlane < 0 && comFromPlane > 0 && velocityIntoPlane < 0))
				{
					numContacts++;
					contact += p;
					contactV += velocityIntoPlane;
				}
			}
		}
		// we've had a hit - typically only two corners can contact 
		if (numContacts > 0)
		{
			// get the average collision velocity into the plane 
			// (covers linear and rotational velocity of all corners involved)
			float collisionV = contactV / (float)numContacts;
			// get the acceleration required to stop (restitution = 0) or reverse
			// (restitution = 1) the average velocity into the plane 
			glm::vec2 acceleration = -plane->getNormal() * ((1.0f + box->getElasticity()) * collisionV);
			// and the average position at which we'll apply the force
			// (corner or edge centre)
			glm::vec2 localContact = (contact / (float)numContacts) - box->getPosition();
			// this is the perpendicular distance we apply the force at relative to 
			// the COM, so Torque = F*r 
			float r = glm::dot(localContact, glm::vec2(plane->getNormal().y, -plane->getNormal().x));
			// work out the "effective mass" - this is a combination of moment of // inertia and mass, and tells us how much the contact point velocity // will change with the force we're applying
			float mass0 = 1.0f / (1.0f / box->getMass() + (r*r) / box->getMoment());
			// and apply the force 
			box->applyForce(acceleration * mass0, localContact);
		}
	}
	return false;
}

bool PhysicsScene::box2Sphere(PhysicsObject *obj1, PhysicsObject *obj2)
{
	Box* box = dynamic_cast<Box*>(obj1);
	Sphere* sphere = dynamic_cast<Sphere*>(obj2);
	if (box != nullptr && sphere != nullptr)
	{
		glm::vec2 circlePos = sphere->getPosition() - box->getPosition();
		float w2 = box->getWidth() / 2, h2 = box->getHeight() / 2;
		int numContacts = 0;
		glm::vec2 contact(0, 0);
		// contact is in our box coordinates 
		// check the four corners to see if any of them are inside the circle
		for (float x = -w2; x <= w2; x += box->getWidth())
		{
			for (float y = -h2; y <= h2; y += box->getHeight())
			{
				glm::vec2 p = x * box->getLocalX() + y * box->getLocalY();
				glm::vec2 dp = p - circlePos;
				if (dp.x*dp.x + dp.y*dp.y < sphere->getRadius()*sphere->getRadius())
				{
					numContacts++; contact += glm::vec2(x, y);
				}
			}
		}
		glm::vec2* direction = nullptr;
		// get the local position of the circle centre 
		glm::vec2 localPos(glm::dot(box->getLocalX(), circlePos), glm::dot(box->getLocalY(), circlePos));
		if (localPos.y < h2 && localPos.y > -h2)
		{
			if (localPos.x > 0 && localPos.x < w2 + sphere->getRadius())
			{
				numContacts++;
				contact += glm::vec2(w2, localPos.y);
				direction = new glm::vec2(box->getLocalX());
			}
			if (localPos.x < 0 && localPos.x > -(w2 + sphere->getRadius()))
			{
				numContacts++;
				contact += glm::vec2(-w2, localPos.y);
				direction = new glm::vec2(-box->getLocalX());
			}
		}
		if (localPos.x < w2 && localPos.x > -w2)
		{
			if (localPos.y > 0 && localPos.y < h2 + sphere->getRadius())
			{
				numContacts++;
				contact += glm::vec2(localPos.x, h2);
				direction = new glm::vec2(box->getLocalY());
			}
			if (localPos.y < 0 && localPos.y > -(h2 + sphere->getRadius()))
			{
				numContacts++;
				contact += glm::vec2(localPos.x, -h2);
				direction = new glm::vec2(-box->getLocalY());
			}
		}
		if (numContacts > 0)
		{
			// average, and convert back into world coords 
			contact = box->getPosition() + (1.0f / numContacts) * (box->getLocalX() *contact.x + box->getLocalY()*contact.y);
			box->ResolveCollision(sphere, contact, direction);
		}
		delete direction;
	}
	return false;
}


bool PhysicsScene::box2Box(PhysicsObject *obj1, PhysicsObject *obj2)
{
	Box* box1 = dynamic_cast<Box*>(obj1);
	Box* box2 = dynamic_cast<Box*>(obj2);
	if (box1 != nullptr && box2 != nullptr)
	{
		glm::vec2 boxPos = box2->getCenter() - box1->getCenter();
		glm::vec2 norm(0, 0);
		glm::vec2 contact(0, 0);
		float pen = 0;
		int numContacts = 0;
		box1->checkBoxCorners(*box2, contact, numContacts, pen, norm); 
		if (box2->checkBoxCorners(*box1, contact, numContacts, pen, norm))
		{
			norm = -norm; 
		} 
		if (pen > 0) 
		{ 
			box1->ResolveCollision(box2, contact / float(numContacts), &norm); 
		}
		return true; 
	} 
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
			//aabb->ResolveCollision(sphere);
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
		if (!collision)
		{
			//aabb1->ResolveCollision(aabb2);
		}
	}
	return false;
}



