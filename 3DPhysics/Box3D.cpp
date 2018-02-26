#include "Box3D.h"


Box3D::Box3D(glm::vec3 position, glm::vec3 extents, glm::vec3 velocity, float mass, bool kinematic, float linearDrag, float angularDrag, float elasticity, glm::vec4 colour)
	: RigidBody3D(BOX, position, velocity, velocity, mass, kinematic, 0, 0, 0)
{
	this->extents = extents;
	this->colour = colour;
	this->m_linearDrag = linearDrag;
	this->m_angularDrag = angularDrag;
	this->m_elasticity = elasticity;
	m_angularVelocity = glm::vec3();
	this->m_moment = 1.0f / 12.0f * m_mass * extents.x * extents.y;
}

Box3D::~Box3D()
{
}

void Box3D::makeGizmo()
{
	aie::Gizmos::addAABBFilled(m_position, extents, colour);
}

bool Box3D::checkCollision(PhysicsObject3D * pOther)
{
	return false;
}

void Box3D::fixedUpdate(glm::vec3 gravity, float timeStep)
{
	RigidBody3D::fixedUpdate(gravity, timeStep);

	// store the local axes
	float cs = cosf(m_rotation.x);
	float sn = sinf(m_rotation.x);

	m_localX = glm::normalize(glm::vec3(cs, sn, 0));
	m_localY = glm::normalize(glm::vec3(-sn, cs, 0));
}

bool Box3D::checkBoxCorners(Box3D & box, glm::vec3 & contact, int & numContacts, float & pen, glm::vec3 & edgeNormal, glm::vec3 & contactForce)
{
	float boxW = box.getExtents().x * 2;
	float boxH = box.getExtents().y * 2;
	float penetration = 0;
	for (float x = -box.getExtents().x; x < boxW; x += boxW)
	{
		for (float y = -box.getExtents().y; y < boxH; y += boxH)
		{
			// pos in worldspace 
			glm::vec3 p = box.m_position + x * box.m_localX + y * box.m_localY;
			// position in our box's space
			glm::vec3 p0(glm::dot(p - m_position, m_localX), glm::dot(p - m_position, m_localY), m_position.z);
			float w2 = extents.x, h2 = extents.y;
			if (p0.y < h2 && p0.y > -h2)
			{
				if (p0.x > 0 && p0.x < w2)
				{
					numContacts++;
					contact += m_position + w2 * m_localX + p0.y * m_localY;
					edgeNormal = m_localX; penetration = w2 - p0.x;
				}
				if (p0.x < 0 && p0.x > -w2)
				{
					numContacts++;
					contact += m_position - w2 * m_localX + p0.y * m_localY;
					edgeNormal = -m_localX; penetration = w2 + p0.x;
				}
			}
			if (p0.x < w2 && p0.x > -w2)
			{
				if (p0.y > 0 && p0.y < h2)
				{
					numContacts++;
					contact += m_position + p0.x * m_localX + h2 * m_localY;
					float pen0 = h2 - p0.y;
					if (pen0 < penetration || penetration == 0)
					{
						penetration = pen0;
						edgeNormal = m_localY;
					}
				}
				if (p0.y < 0 && p0.y > -h2)
				{
					numContacts++;
					contact += m_position + p0.x * m_localX - h2 * m_localY;
					float pen0 = h2 + p0.y;
					if (pen0 < penetration || penetration == 0)
					{
						penetration = pen0;
						edgeNormal = -m_localY;
					}
				}
			}
		}
	}
	contactForce = penetration * edgeNormal;
	return (penetration != 0);
}
