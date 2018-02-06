#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"
#include "Sphere.h"

class PhysicsMaybeApp : public aie::Application {
public:

	PhysicsMaybeApp();
	virtual ~PhysicsMaybeApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void setupContinuousDemo(glm::vec2 position, float angle, float speed, float gravity);

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	Sphere* rocket;
	PhysicsScene*		m_physicsScene;
};