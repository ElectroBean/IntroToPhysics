#include "PhysicsMaybeApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <glm/ext.hpp>
#include <Gizmos.h>
#include "Sphere.h"
#include "Plane.h"
#include "AABB.h"
#include "Box.h"
#include <iostream>

PhysicsMaybeApp::PhysicsMaybeApp() {

}

PhysicsMaybeApp::~PhysicsMaybeApp() {

}

bool PhysicsMaybeApp::startup() {

	m_2dRenderer = new aie::Renderer2D();

	//gizmos
	aie::Gizmos::create(255U, 255U, 65535U, 35535U);

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);


	m_physicsScene = new PhysicsScene();
	//set grav
	m_physicsScene->setGravity(glm::vec2(0, -9.8f));
	m_physicsScene->setTimeStep(0.0128f);

	//Sphere* ball;
	//ball = new Sphere(glm::vec2(-40, 20), glm::vec2(10, 0), 3.0f, false, 5, 0.2f, 0.2f, 1.0f, glm::vec4(1, 0, 0, 1));
	//m_physicsScene->addActor(ball);
	//
	//Sphere* ball2;
	//ball2 = new Sphere(glm::vec2(0, 15), glm::vec2(-10, 0), 3.0f, false, 5, 0.2f, 0.2f, 1.0f, glm::vec4(1, 1, 0, 1));
	//m_physicsScene->addActor(ball2);
	//
	//Sphere* ball3;
	//ball3 = new Sphere(glm::vec2(15, 20), glm::vec2(-10, 0), 3.0f, false, 5, 0.2f, 0.2f, 1.0f, glm::vec4(0,0, 1, 1));
	//m_physicsScene->addActor(ball3);

	//Box* box1;
	//box1 = new Box(glm::vec2(15, 20), glm::vec2(5, 5), glm::vec2(0, 0), 5.0f, false, 0.2f, 0.2f, 1.0f, glm::vec4(1, 0, 0, 1));
	//m_physicsScene->addActor(box1);
	//
	//Box* box2;
	//box2 = new Box(glm::vec2(-10, 20), glm::vec2(5, 5), glm::vec2(0, 0), 5.0f, false, 0.2f, 0.2f, 1.0f, glm::vec4(1, 0, 0, 1));
	//m_physicsScene->addActor(box2);

	Plane* plane1;
	plane1 = new Plane(glm::vec2(0.8f, 1), -20);
	m_physicsScene->addActor(plane1);

	Plane* plane2;
	plane2 = new Plane(glm::vec2(-0.8f, 1), -20);
	m_physicsScene->addActor(plane2);

	Plane* plane3;
	plane3 = new Plane(glm::vec2(0, 1), -20);
	m_physicsScene->addActor(plane3);

	//AABB* aabb1;
	//aabb1 = new AABB(glm::vec2(10, 10), glm::vec2(5, 5), glm::vec2(0, 0), 5.0f, false, 0.2f, 0.2f, 1.0f, glm::vec4(0, 1, 0, 1));
	//m_physicsScene->addActor(aabb1);
	//
	//AABB* aabb2;
	//aabb2 = new AABB(glm::vec2(10, 40), glm::vec2(5, 5), glm::vec2(0, 0), 5.0f, false, 0.2f, 0.2f, 1.0f, glm::vec4(0, 1, 0, 1));
	//m_physicsScene->addActor(aabb2);

	float radius = 1.0f;
	float speed = 30;
	glm::vec2 startPos(-40, 0);
	float inclination = 3.1415f / 4.0f; //45 degrees

	gui = new GUI();

	//m_physicsScene->addActor(new Sphere(startPos, inclination, speed, 1, radius, glm::vec4(1, 0, 0, 1)));
	//
	//setupContinuousDemo(glm::vec2(-40, 0), 3.1415f / 4, 30.0f, -9.8f);
	//rocket = new Sphere(glm::vec2(-40, 0), glm::vec2(12, 10), 100.0f, true, 5, glm::vec4(1, 1, 0, 1));
	//m_physicsScene->addActor(rocket);

	return true;
}

void PhysicsMaybeApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
	aie::Gizmos::destroy(); 
	delete gui;
}


float Timer = 0.0f;
float TimeDelay;
float spawnTimer = 0.0f;
float spawnDelay = 1.5f;
void PhysicsMaybeApp::update(float deltaTime) {

	gui->update(&TimeDelay, m_physicsScene->getGravityP());

	// input example
	aie::Input* input = aie::Input::getInstance();
	Timer -= deltaTime;

	spawnTimer -= deltaTime;

	if (input->wasMouseButtonPressed(1))
	{
		if (spawnTimer < 0)
		{
			Sphere* newSphere = new Sphere(glm::vec2(-50, 20), glm::vec2(10, 0), 10.0f, false, 5.0f, 0.2f, 0.2f, 1.0f, glm::vec4(1, 0, 0, 1));
			m_physicsScene->addActor(newSphere);
			spawnTimer = spawnDelay;
		}
	}

	//setupContinuousDemo(rocket->getPosition(), 3.1415f / 4, 30.0f, -9.8f);

	//if (input->isKeyDown(32))
	//{
	//	rocket->setKinematic(false);
	//	if (Timer <= 0)
	//	{
	//		if (rocket->getMass() > 0.1f)
	//		{
	//			srand((unsigned)time(0));
	//
	//			float r = rand() % 2 + 0.1f;
	//			float g = rand() % 2 + 0.1f;
	//			float b = rand() % 2 + 0.1f;
	//			Sphere* newThing;
	//			newThing = new Sphere(glm::vec2(rocket->getPosition().x, rocket->getPosition().y - 7), glm::vec2(0, 0), 5, false, 5, glm::vec4(r, g, b, 0.3f));
	//			m_physicsScene->addActor(newThing);
	//
	//			rocket->applyForceToActor(newThing, glm::vec2(0, -50.0f));
	//			rocket->changeMass(-0.1f);
	//
	//			Timer = TimeDelay;
	//		}
	//	}
	//
	//}

	//clear gizmo
	aie::Gizmos::clear();
	//
	////draw grid of blocks and paddles
	//static const glm::vec4 colours[] = {
	//	glm::vec4(1, 0, 0, 1), glm::vec4(0, 1, 0, 1),
	//	glm::vec4(0, 0, 1, 1), glm::vec4(0.8f, 0, 0.5f, 1),
	//	glm::vec4(0, 1, 1, 1)
	//};
	//
	//static const int rows = 5;
	//static const int columns = 10;
	//static const int hSpace = 1;
	//static const int vSpace = 1;
	//
	//static const glm::vec2 scrExtents(100, 50);
	//static const glm::vec2 boxExtents(7, 3);
	//static const glm::vec2 startPos(-(columns >> 1)*((boxExtents.x * 2) + vSpace) + boxExtents.x + (vSpace / 2.0f),
	//	scrExtents.y - ((boxExtents.y * 2) + hSpace));

	//draw grid of blocks
	//glm::vec2 pos;
	//for (int i = 0; i < rows; i++)
	//{
	//	pos = glm::vec2(startPos.x, startPos.y - (i * ((boxExtents.y * 2) + hSpace)));
	//	for (int j = 0; j < columns; j++)
	//	{
	//		aie::Gizmos::add2DAABBFilled(pos, boxExtents, colours[i]);
	//		aie::Gizmos::add2DTri(pos + glm::vec2(-0.1f, 0), pos + glm::vec2(0, 0.1f), pos + glm::vec2(0.1f, 0), colours[i]);
	//		pos.x += (boxExtents.x * 2) + vSpace;
	//	}
	//}
	//
	////draw ball
	//aie::Gizmos::add2DCircle(glm::vec2(0, -45), 3, 12, glm::vec4(1, 1, 0, 1));
	//
	////draw paddle
	//aie::Gizmos::add2DAABBFilled(glm::vec2(0, -50), glm::vec2(12, 2), glm::vec4(1, 0, 1, 1));
	//
	////draw box
	//aie::Gizmos::add2DAABB(glm::vec2(0, -25), glm::vec2(20, 10), glm::vec4(1, 3, 1, 1));
	//aie::Gizmos::add2DAABBFilled(glm::vec2(0, -31.7f), glm::vec2(20, 3.3f), glm::vec4(1, 1, 0, 1));
	//aie::Gizmos::add2DAABBFilled(glm::vec2(0, -25.1f), glm::vec2(20, 3.3f), glm::vec4(1, 0, 0, 1));
	//
	////draw box
	//aie::Gizmos::add2DAABBFilled(glm::vec2(-50, -25), glm::vec2(20, 10), glm::vec4(1, 3, 1, 1));
	//aie::Gizmos::add2DCircle(glm::vec2(-50, -25), 5, 12, glm::vec4(1, 0, 0, 1));
	//
	//
	//aie::Gizmos::add2DCircle(glm::vec2(50, -25), 30, 100, glm::vec4(1, 0, 0, 1));
	//aie::Gizmos::add2DCircle(glm::vec2(50, -25), 28, 100, glm::vec4(1, 0.5, 0, 1));
	//aie::Gizmos::add2DCircle(glm::vec2(50, -25), 26, 100, glm::vec4(1, 1, 0, 1));
	//aie::Gizmos::add2DCircle(glm::vec2(50, -25), 24, 100, glm::vec4(0, 1, 0, 1));
	//aie::Gizmos::add2DCircle(glm::vec2(50, -25), 22, 100, glm::vec4(0, 0, 1, 1));
	//aie::Gizmos::add2DCircle(glm::vec2(50, -25), 20, 100, glm::vec4(1, 0, 1, 1));
	//aie::Gizmos::add2DCircle(glm::vec2(50, -25), 18, 100, glm::vec4(1, 0.5, 1, 1));
	//aie::Gizmos::add2DCircle(glm::vec2(50, -25), 16, 100, glm::vec4(0, 0, 0, 1));
	//aie::Gizmos::add2DAABBFilled(glm::vec2(50, -50), glm::vec2(30, 25), glm::vec4(0, 0, 0, 1));

	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsMaybeApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();
	// draw your stuff here!
	static float aspectRatio = 16 / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100,
		-100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));

	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}

void PhysicsMaybeApp::setupContinuousDemo(glm::vec2 position, float angle, float speed, float gravity)
{
	float t = 0;
	float tStep = 0.5f;
	float radius = 1.0f;
	int segments = 12;
	glm::vec4 colour = glm::vec4(1, 1, 0, 1);
	while (t <= 5)
	{
		glm::vec2 inc = glm::vec2(sin(angle), cos(angle));

		// calculate the x, y position of the projectile at time t
		float x = position.x + (inc.x * speed * t);
		float y = position.y + (inc.y * speed * t) + ((gravity * (t*t)) / 2.0f);

		aie::Gizmos::add2DCircle(glm::vec2(x, y), radius, segments, colour);
		t += tStep;
	}

}

