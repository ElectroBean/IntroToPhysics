#pragma once
#include <glm/ext.hpp>
	enum ShapeType {
		PLANE = 0,
		SPHERE = 1,
		BOX = 2,
		aabb = 3,
		SHAPE_COUNT,
	};
class PhysicsObject3D
{

protected:
	PhysicsObject3D(ShapeType a_shapeID) : m_shapeID(a_shapeID) {}
	~PhysicsObject3D();

public:
	PhysicsObject3D();
	virtual void fixedUpdate(glm::vec3 gravity, float timeStep) = 0;
	virtual void debug() = 0;
	virtual void makeGizmo() = 0;
	virtual void resetPosition() {};
	ShapeType getShapeID() { return m_shapeID; }
protected:
	ShapeType m_shapeID;
};

