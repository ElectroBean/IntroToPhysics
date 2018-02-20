
#pragma once
#include <glm/ext.hpp>

enum ShapeType {
	JOINT = -1,
	PLANE = 0, 
	SPHERE = 1, 
	BOX = 2,
	aabb = 3,
	SHAPE_COUNT,
};

class PhysicsObject
{
protected:
	PhysicsObject(ShapeType a_shapeID)  : m_shapeID(a_shapeID) {}
public:
	PhysicsObject();
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void debug() = 0;
	virtual void makeGizmo() = 0;
	virtual void resetPosition() {};
	ShapeType getShapeID() { return m_shapeID; }
protected:
	ShapeType m_shapeID;
};

