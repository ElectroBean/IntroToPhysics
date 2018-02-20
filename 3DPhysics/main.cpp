#include "_3DPhysicsApp.h"

int main() {
	
	// allocation
	auto app = new _3DPhysicsApp();

	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}