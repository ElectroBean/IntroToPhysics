#include "Box.h"



Box::Box(glm::vec2 position, glm::vec2 extents, glm::vec2 velocity, float mass, bool kinematic, float linearDrag, float angularDrag, float elasticity, glm::vec4 colour)
	: RigidBody(BOX, position, velocity, 0, mass, kinematic, 0, 0, 0)
{
	this->extents = extents;
	this->colour = colour;
	this->m_linearDrag = linearDrag;
	this->m_angularDrag = angularDrag;
	this->m_elasticity = elasticity;
	m_angularVelocity = 0.0f;
	this->m_moment = 1.0f / 12.0f * m_mass * extents.x * extents.y;
}


Box::~Box()
{

}

void Box::makeGizmo()
{
	// if only using rotation 
	// glm::mat4 transform = glm::rotate(m_rotation, glm::vec3(0, 0, 1)); 
	// aie::Gizmos::add2DAABBFilled(getCenter(), 
			// m_extents, m_colour, &transform); 
	// draw using local axes 
	glm::vec2 p1 = m_position - m_localX * extents.x - m_localY * extents.y;
	glm::vec2 p2 = m_position + m_localX * extents.x - m_localY * extents.y;
	glm::vec2 p3 = m_position - m_localX * extents.x + m_localY * extents.y;
	glm::vec2 p4 = m_position + m_localX * extents.x + m_localY * extents.y;
	aie::Gizmos::add2DTri(p1, p2, p4, colour);
	aie::Gizmos::add2DTri(p1, p4, p3, colour);
}

bool Box::checkCollision(PhysicsObject * pOther)
{
	return false;
}

void Box::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	RigidBody::fixedUpdate(gravity, timeStep);

	// store the local axes
	float cs = cosf(m_rotation);
	float sn = sinf(m_rotation);

	m_localX = glm::normalize(glm::vec2(cs, sn));
	m_localY = glm::normalize(glm::vec2(-sn, cs));
}

bool Box::checkBoxCorners(Box& box, glm::vec2& contact, int& numContacts, float& pen, glm::vec2& edgeNormal, 
							glm::vec2& contactForce)
{
	float boxW = box.getExtents().x * 2;
	float boxH = box.getExtents().y * 2;
	float penetration = 0;
	for (float x = -box.getExtents().x; x < boxW; x += boxW) 
	{
		for (float y = -box.getExtents().y; y < boxH; y += boxH)
		{ 
			// pos in worldspace 
			glm::vec2 p = box.m_position + x * box.m_localX + y * box.m_localY;
			// position in our box's space
			glm::vec2 p0(glm::dot(p - m_position, m_localX), glm::dot(p - m_position, m_localY));
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