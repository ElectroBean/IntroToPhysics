#include "PhysicsScene3D.h"
#include <list>
#include <iostream>
#include "Sphere3D.h"
#include "Plane3D.h"
#include "Box3D.h"


PhysicsScene3D::PhysicsScene3D()
{
	setTimeStep(0.01f);
	setGravity(glm::vec3(-9.8f, 0, 0));
}


PhysicsScene3D::~PhysicsScene3D()
{
	for (auto pActor : m_actors)
	{
		
	}
}

void PhysicsScene3D::addActor(PhysicsObject3D * actor)
{
	m_actors.push_back(actor);
}

void PhysicsScene3D::removeActor(PhysicsObject3D * actor)
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

void PhysicsScene3D::update(float dt)
{
	static std::list<PhysicsObject3D*> dirty;

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
	}
}

void PhysicsScene3D::debugScene()
{
	int count = 0;
	for (auto pActor : m_actors) {
		std::cout << count << " : ";
		pActor->debug();
		count++;
	}
}

void PhysicsScene3D::updateGizmos()
{
	for (auto pActor : m_actors)
	{
		pActor->makeGizmo();
	}
}

typedef bool(*fn)(PhysicsObject3D*, PhysicsObject3D*);

static fn collisionFunctionArray[] =
{
	PhysicsScene3D::plane2Plane, PhysicsScene3D::plane2Sphere, PhysicsScene3D::plane2Box, PhysicsScene3D::plane2aabb,
	PhysicsScene3D::sphere2Plane, PhysicsScene3D::sphere2Sphere, PhysicsScene3D::sphere2Box, PhysicsScene3D::sphere2aabb,
	PhysicsScene3D::box2Plane, PhysicsScene3D::box2Sphere, PhysicsScene3D::box2Box, PhysicsScene3D::box2aabb,
	PhysicsScene3D::aabb2Plane, PhysicsScene3D::aabb2Sphere, PhysicsScene3D::aabb2Box, PhysicsScene3D::aabb2aabb,
};

void PhysicsScene3D::checkForCollision()
{
	int actorCount = m_actors.size();

	//need to check for collisions against all object except this one
	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsObject3D* object1 = m_actors[outer];
			PhysicsObject3D* object2 = m_actors[inner];
			int shapeId1 = object1->getShapeID();
			int shapeId2 = object2->getShapeID();

			if (shapeId1 < 0 || shapeId2 < 0)
				continue;
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

void PhysicsScene3D::DestroyAll()
{
}

bool PhysicsScene3D::plane2Plane(PhysicsObject3D *, PhysicsObject3D *)
{
	return false;
}

bool PhysicsScene3D::plane2Sphere(PhysicsObject3D *obj1, PhysicsObject3D *obj2)
{
	return sphere2Plane(obj2, obj1);
}

bool PhysicsScene3D::plane2Box(PhysicsObject3D *obj1, PhysicsObject3D *obj2)
{
	box2Plane(obj2, obj1);
	return false;
}

bool PhysicsScene3D::plane2aabb(PhysicsObject3D *, PhysicsObject3D *)
{
	return false;
}

bool PhysicsScene3D::sphere2Plane(PhysicsObject3D *obj1, PhysicsObject3D *obj2)
{
	Sphere3D *sphere = dynamic_cast<Sphere3D*>(obj1);
	Plane3D *plane = dynamic_cast<Plane3D*>(obj2);
	//if we are successful then test for collision 
	if (sphere != nullptr && plane != nullptr)
	{
		glm::vec3 collisionNormal = plane->getNormal();
		float sphereToPlane = glm::dot(sphere->getPosition(), plane->getNormal()) - plane->getDistance();
		// if we are behind plane then we flip the normal 
		if (sphereToPlane < 0)
		{
			collisionNormal *= -1; sphereToPlane *= -1;
		}
		float intersection = sphere->getRadius() - sphereToPlane;
		if (intersection > 0)
		{
			glm::vec3 contact = sphere->getPosition() + (collisionNormal * -sphere->getRadius());
			sphere->setPosition(sphere->getPosition() + plane->getNormal() * (sphere->getRadius() - sphereToPlane));
			plane->ResolveCollision(sphere, contact);
			return true;
		}
	}
	
	return false;
}

bool PhysicsScene3D::sphere2Sphere(PhysicsObject3D *, PhysicsObject3D *)
{
	return false;
}

bool PhysicsScene3D::sphere2Box(PhysicsObject3D *obj1, PhysicsObject3D *obj2)
{
	box2Sphere(obj2, obj1);
	return false;
}

bool PhysicsScene3D::sphere2aabb(PhysicsObject3D *, PhysicsObject3D *)
{
	return false;
}

bool PhysicsScene3D::box2Plane(PhysicsObject3D *obj1, PhysicsObject3D *obj2)
{
	Box3D *box = dynamic_cast<Box3D*>(obj1);
	Plane3D *plane = dynamic_cast<Plane3D*>(obj2);
	//if we are successful then test for collision 
	//if (box != nullptr && plane != nullptr)
	//{
	//	int numContacts = 0;
	//	glm::vec3 contact(0, 0, 0);
	//	float contactV = 0;
	//	float radius = 0.5f * std::fminf(box->getWidth(), box->getHeight());
	//	float penetration = 0.0f;
	//	// which side is the centre of mass on? 
	//	glm::vec3 planeOrigin = plane->getNormal() * plane->getDistance();
	//	float comFromPlane = glm::dot(box->getPosition() - planeOrigin, plane->getNormal());
	//	// check all four corners to see if we've hit the plane 
	//	for (float x = -box->getExtents().x; x < box->getWidth(); x += box->getWidth())
	//	{
	//		for (float y = -box->getExtents().y; y < box->getHeight(); y += box->getHeight())
	//		{
	//			// get the position of the corner in world space
	//			glm::vec3 p = box->getPosition() + x * box->getLocalX() + y * box->getLocalY();
	//			float distFromPlane = glm::dot(p - planeOrigin, plane->getNormal());
	//			// this is the total velocity of the point 
	//			float velocityIntoPlane = glm::dot(box->getVelocity() + box->getRotation() * (-y * box->getLocalX() + x * box->getLocalY()), plane->getNormal());
	//			// if this corner is on the opposite side from the COM, 
	//			// and moving further in, we need to resolve the collision
	//			if ((distFromPlane > 0 && comFromPlane < 0 && velocityIntoPlane >= 0) ||
	//				(distFromPlane < 0 && comFromPlane > 0 && velocityIntoPlane <= 0))
	//			{
	//				numContacts++;
	//				contact += p;
	//				contactV += velocityIntoPlane;
	//
	//				if (comFromPlane >= 0)
	//				{
	//					if (penetration > distFromPlane)
	//					{
	//						penetration = distFromPlane;
	//					}
	//					else
	//					{
	//						if (penetration < distFromPlane)
	//						{
	//							penetration = distFromPlane;
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}
	//	// we've had a hit - typically only two corners can contact 
	//	if (numContacts > 0)
	//	{
	//		// get the average collision velocity into the plane 
	//		// (covers linear and rotational velocity of all corners involved)
	//		float collisionV = contactV / (float)numContacts;
	//		// get the acceleration required to stop (restitution = 0) or reverse
	//		// (restitution = 1) the average velocity into the plane 
	//		glm::vec3 acceleration = -plane->getNormal() * ((1.0f + box->getElasticity()) * collisionV);
	//		// and the average position at which we'll apply the force
	//		// (corner or edge centre)
	//		glm::vec3 localContact = (contact / (float)numContacts) - box->getPosition();
	//		// this is the perpendicular distance we apply the force at relative to 
	//		// the COM, so Torque = F*r 
	//		float r = glm::dot(localContact, glm::vec3(plane->getNormal().y, -plane->getNormal().x, 0));
	//		// work out the "effective mass" - this is a combination of moment of // inertia and mass, and tells us how much the contact point velocity // will change with the force we're applying
	//		float mass0 = 1.0f / (1.0f / box->getMass() + (r*r) / box->getMoment());
	//		// and apply the force 
	//		box->applyForce(acceleration * mass0, localContact);
	//		box->setPosition(box->getPosition() - plane->getNormal() * penetration);
	//
	//	}
	//}

	if (box != nullptr && plane != nullptr) {

		int numContacts = 0;
		glm::vec3 contact(0, 0, 0);
		float contactV = 0;
		float radius = 0.5f * fminf(box->getWidth(), box->getHeight());
		float penetration = 0;

		// Which side is the center of mass on?
		glm::vec3 planeOrigin = plane->getNormal() * plane->getDistance();
		float comFromPlane = dot(box->getPosition() - planeOrigin, plane->getNormal());
		// Check all four corners to see if it has hit the plane
		for (float x = -box->getExtents().x; x <= box->getWidth(); x += box->getWidth()) {

			for (float y = -box->getExtents().y; y <= box->getHeight(); y += box->getHeight()) {

				// Get the position of the corner in world space
				glm::vec3 p = box->getPosition() + x * box->getLocalX() + y * box->getLocalY();

				float distFromPlane = dot(p - planeOrigin, plane->getNormal());

				// This is the total velocity of the point
				float velocityIntoPlane = dot(box->getVelocity() + box->getRotation() *
					(-y * box->getLocalY() + x * box->getLocalY()), plane->getNormal());

				// If this corner is on the opposite side from the COM
				// and is moving further in, we need to resolve the collision
				if ((distFromPlane > 0 && comFromPlane < 0 && velocityIntoPlane >= 0) ||
					(distFromPlane < 0 && comFromPlane > 0 && velocityIntoPlane <= 0)) {

					numContacts++;
					contact += p;
					contactV += velocityIntoPlane;

					if (comFromPlane >= 0) {

						if (penetration > distFromPlane) penetration = distFromPlane;
					}

					else {

						if (penetration < distFromPlane) penetration = distFromPlane;
					}
				}
			}
		}
		// We've had a hit - typically only two corners can make contact
		if (numContacts > 0) {

			// Get the average collision velocity into the plane
			// (covers linear and rotational velocity of all corners involved)
			float collisionV = contactV / float(numContacts);

			// Get the acceleration required to stop (restitution = 0) or reverse
			// (restitution = 1) the average velocity into the plane
			glm::vec3 acceleration = -plane->getNormal() * ((1.0f + box->getElasticity()) * collisionV);

			// And the average position at which we'll apply the force
			// (corner or edge center)
			glm::vec3 localContact = (contact / float(numContacts)) - box->getPosition();

			// This is the perpendicular distance we apply the force at relative to 
			// the COM, so Torque = F*r
			float r = dot(localContact, glm::vec3(plane->getNormal().y, plane->getNormal().x, 0));

			// Work out the "effective mass" - this is a combination of moment of
			// inertia & mass and tells us how much the cotact point velocity
			// will change with the force we're applying
			float mass = 1.0f / (1.0f / box->getMass() + (r * r) / box->getMoment());

			// Apply the force
			box->setPosition(box->getPosition() - plane->getNormal() * penetration);
			box->applyForce(acceleration * mass, localContact);
			return true;
		}
	}

	return false;
}

bool PhysicsScene3D::box2Sphere(PhysicsObject3D *obj1, PhysicsObject3D *obj2)
{
	Box3D* box = dynamic_cast<Box3D*>(obj1);
	Sphere3D* sphere = dynamic_cast<Sphere3D*>(obj2);
	if (box != nullptr && sphere != nullptr)
	{
		glm::vec3 circlePos = sphere->getPosition() - box->getPosition();
		float w2 = box->getWidth() / 2, h2 = box->getHeight() / 2;
		int numContacts = 0;
		glm::vec3 contact(0, 0, 0);
		// contact is in our box coordinates 
		// check the four corners to see if any of them are inside the circle
		for (float x = -w2; x <= w2; x += box->getWidth())
		{
			for (float y = -h2; y <= h2; y += box->getHeight())
			{
				glm::vec3 p = x * box->getLocalX() + y * box->getLocalY();
				glm::vec3 dp = p - circlePos;
				if (dp.x*dp.x + dp.y*dp.y < sphere->getRadius()*sphere->getRadius())
				{
					numContacts++;
					contact += glm::vec3(x, y, 0);
				}
			}
		}
		glm::vec3* direction = nullptr;
		// get the local position of the circle centre 
		glm::vec3 localPos(glm::dot(box->getLocalX(), circlePos), glm::dot(box->getLocalY(), circlePos), 0);
		if (localPos.y < h2 && localPos.y > -h2)
		{
			if (localPos.x > 0 && localPos.x < w2 + sphere->getRadius())
			{
				numContacts++;
				contact += glm::vec3(w2, localPos.y, localPos.z);
				direction = new glm::vec3(box->getLocalX());
			}
			if (localPos.x < 0 && localPos.x > -(w2 + sphere->getRadius()))
			{
				numContacts++;
				contact += glm::vec3(-w2, localPos.y, localPos.z);
				direction = new glm::vec3(-box->getLocalX());
			}
		}
		if (localPos.x < w2 && localPos.x > -w2)
		{
			if (localPos.y > 0 && localPos.y < h2 + sphere->getRadius())
			{
				numContacts++;
				contact += glm::vec3(localPos.x, h2, localPos.z);
				direction = new glm::vec3(box->getLocalY());
			}
			if (localPos.y < 0 && localPos.y > -(h2 + sphere->getRadius()))
			{
				numContacts++;
				contact += glm::vec3(localPos.x, -h2, localPos.z);
				direction = new glm::vec3(-box->getLocalY());
			}
		}
		if (numContacts > 0)
		{
	
			// average, and convert back into world coords 
			contact = box->getPosition() + (1.0f / numContacts) * (box->getLocalX() *contact.x + box->getLocalY()*contact.y);
	
	
			glm::vec3 A = contact;
			glm::vec3 V = A - sphere->getPosition();
			glm::vec3 normal = glm::normalize(V);
	
	
			if (!box->getIsKinematic() && !sphere->getIsKinematic())
			{
				sphere->setPosition(sphere->getPosition() - normal * (sphere->getRadius() - length(V)));
				box->setPosition(box->getPosition() + normal * (sphere->getRadius() - length(V)));
			}
			else if (!box->getIsKinematic())
			{
				box->setPosition(box->getPosition() + normal * (sphere->getRadius() - length(V)));
			}
			else
			{
				sphere->setPosition(sphere->getPosition() - normal * (sphere->getRadius() - length(V)));
			}
	
			box->ResolveCollision(sphere, contact, direction);
		}
		delete direction;
	}
	return false;
}

bool PhysicsScene3D::box2Box(PhysicsObject3D *obj1, PhysicsObject3D *obj2)
{
	Box3D* box1 = dynamic_cast<Box3D*>(obj1);
	Box3D* box2 = dynamic_cast<Box3D*>(obj2);
	if (box1 != nullptr && box2 != nullptr)
	{
		glm::vec3 boxPos = box2->getCenter() - box1->getCenter();
		glm::vec3 norm(0, 0, 0);
		glm::vec3 contactForce1, contactForce2;
		glm::vec3 contact(0, 0, 0);
		float pen = 0;
		int numContacts = 0;

		box1->checkBoxCorners(*box2, contact, numContacts, pen, norm, contactForce1);

		if (box2->checkBoxCorners(*box1, contact, numContacts, pen, norm, contactForce2))
		{
			norm = -norm;
		}
		if (pen > 0)
		{
			box1->ResolveCollision(box2, contact / float(numContacts), &norm);
		}

		if (numContacts > 0)
		{
			glm::vec3 contactForce = 0.5f*(contactForce1 - contactForce2);

			if (!box1->getIsKinematic() && !box2->getIsKinematic())
			{
				box1->setPosition(box1->getPosition() - contactForce);
				box2->setPosition(box2->getPosition() + contactForce);
			}
			else if (!box1->getIsKinematic())
			{
				box1->setPosition(box1->getPosition() - contactForce);
			}
			else
			{
				box2->setPosition(box2->getPosition() + contactForce);
			}

			box1->ResolveCollision(box2, contact / float(numContacts), &norm);
			return true;
		}
	}
	return false;
}

bool PhysicsScene3D::box2aabb(PhysicsObject3D *, PhysicsObject3D *)
{
	return false;
}

bool PhysicsScene3D::aabb2Plane(PhysicsObject3D *, PhysicsObject3D *)
{
	return false;
}

bool PhysicsScene3D::aabb2Sphere(PhysicsObject3D *, PhysicsObject3D *)
{
	return false;
}

bool PhysicsScene3D::aabb2Box(PhysicsObject3D *, PhysicsObject3D *)
{
	return false;
}

bool PhysicsScene3D::aabb2aabb(PhysicsObject3D *, PhysicsObject3D *)
{
	return false;
}
