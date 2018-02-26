#include "_3DPhysicsApp.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Plane3D.h"
#include "Sphere3D.h"
#include "Box3D.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

_3DPhysicsApp::_3DPhysicsApp() {

}

_3DPhysicsApp::~_3DPhysicsApp() {

}

bool _3DPhysicsApp::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);

	m_physicsScene3D = new PhysicsScene3D();
	m_physicsScene3D->setGravity(glm::vec3(0, -9.8f, 0));
	m_physicsScene3D->setTimeStep(0.015f);


	Plane3D* plane3D;
	plane3D = new Plane3D(glm::vec3(0, 1, 0), 0);
	m_physicsScene3D->addActor(plane3D);

	Sphere3D* sphere3D;
	sphere3D = new Sphere3D(glm::vec3(1.5f, 10, 0), glm::vec3(0, 0, 0), 10.0f, false, 1.0f, 0.1f, 0.1f, 1.0f, glm::vec4(1, 0, 0, 1));
	m_physicsScene3D->addActor(sphere3D);

	Box3D* box3D;
	box3D = new Box3D(glm::vec3(0, 5, 0), glm::vec3(1, 1, 1), glm::vec3(), 10.0f, false, 0.1f, 0.1f, 0.0f, glm::vec4(0, 1, 0, 1));
	m_physicsScene3D->addActor(box3D);

	return true;
}

void _3DPhysicsApp::shutdown() {

	Gizmos::destroy();
}

void _3DPhysicsApp::update(float deltaTime) {

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	//vec4 white(1);
	//vec4 black(0, 0, 0, 1);
	//for (int i = 0; i < 21; ++i) {
	//	Gizmos::addLine(vec3(-10 + i, 0, 10),
	//					vec3(-10 + i, 0, -10),
	//					i == 10 ? white : black);
	//	Gizmos::addLine(vec3(10, 0, -10 + i),
	//					vec3(-10, 0, -10 + i),
	//					i == 10 ? white : black);
	//}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	m_physicsScene3D->update(deltaTime);
	m_physicsScene3D->updateGizmos();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void _3DPhysicsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// update perspective based on screen size
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

	Gizmos::draw(m_projectionMatrix * m_viewMatrix);
}