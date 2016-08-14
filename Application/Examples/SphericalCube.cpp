#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./Pipeline/include/Rendering.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Data/include/OPcman.h"
#include "./Human/include/Rendering/Primitives/OPsphericalCube.h"


// Data for this Game State Example
typedef struct {
	OPsphericalCube SphericalCube;			// The Spherical Cube to render
	OPeffect Effect;		// The Effect used to render the Mesh
	OPcam Camera;			// The Camera to use in the Effect to render the Mesh
	ui32 Rotation;			// The amount to rotate the Mesh
	OPvec3 LightDirection;	// Where the Light Source is coming from
} SphericalCubeExample;

SphericalCubeExample* sphericalCubeExample;

void ExampleSphericalCubeEnter(OPgameState* last) {

	// Allocating a memory block for this example
	sphericalCubeExample = (SphericalCubeExample*)OPallocZero(sizeof(SphericalCubeExample));
	
	// Load up the mesh into the Content Manager
	// If the model was already loaded by a previous Game State
	// it'll continue on without reloading it.
	// OPcamnLoadGet calls OPCMAN.Load and then returns OPCMAN.Get
	// The OPCMAN.Load call ensures that this mesh has been loaded
	// The OPCMAN.Get call returns a pointer to the resource (an OPmesh)
	// that's contained in the Content Manager
	sphericalCubeExample->SphericalCube.Init(10);

	// The effect that will be used to render the mesh
	// The renderGenEffect is a simplified utility method
	// which requires the attributes are given in a set order
	// Position (vec3), then Normal (vec3)
	// For more granular control use OPeffectCreate
	sphericalCubeExample->Effect.Init("SimpleModel.vert", "SimpleModel.frag");

	// Sets up the camera as a perpsective camera for rendering
	sphericalCubeExample->Camera.SetPerspective(
		OPVEC3_ONE * 1,
		OPVEC3_ZERO,
		OPVEC3_UP,
		0.1f,
		1000.0f,
		45.0f,
		OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width / (f32)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height
		);

	// A default light direction used in the effect
	sphericalCubeExample->LightDirection = OPVEC3_UP;

	// This can be controlled in the update loop if it varies
	// Since this is a simple example we'll ensure that it's set
	// to turn the Depth Buffer on and continue
	OPrenderDepth(1);
}

OPint ExampleSphericalCubeUpdate(OPtimer* time) {

	////////////////////////
	// Update
	////////////////////////

	// The application root is set to update the Keyboard, Mouse and GamePads
	// If you need more granular control for when these update, please modify
	// this application's main.cpp
	if (OPkeyboardIsDown(OPkeyboardKey::SPACE)) { sphericalCubeExample->Rotation++; }

	// Generates an OPmat4 (Matrix 4x4) which is rotated on the Y axis
	OPmat4 world = OPmat4RotY(sphericalCubeExample->Rotation / 100.0f);
	//OPmat4Scl(&world, 0.25f, 0.25f, 0.25f);

	////////////////////////
	// Render
	////////////////////////
	OPrenderClear(0.2f, 0.2f, 0.2f);

	for (OPint i = 0; i < 6; i++) {
		// A helper utility which binds the Mesh, Effect and the World, View and Projection Matrices
		// For more granular control please take a look at the Textured Example
		OPbindMeshEffectWorldCam(&sphericalCubeExample->SphericalCube.sides[i], &sphericalCubeExample->Effect, &world, &sphericalCubeExample->Camera);
		
		// Sets the vLightDirection uniform on the Effect that is currently bound (sphericalCubeExample->Effect)
		OPeffectSet("vLightDirection", &sphericalCubeExample->LightDirection);

		// Renders to the screen the currently bound Mesh (sphericalCubeExample->Mesh)
		OPmeshRender();
	}

	// Swaps the back buffer
	OPrenderPresent();

	// Tells the engine to continue running
	// Returning true will tell the engine to terminate
	return false;

}
void ExampleSphericalCubeRender(OPfloat delta) {

}
// The OPifex Engine will call this itself when you call OPgameStateChange
OPint ExampleSphericalCubeExit(OPgameState* next) {
	// Clean up phase for the Game State
	sphericalCubeExample->Effect.Destroy();	
	OPfree(sphericalCubeExample);
	return 0;
}


OPint GS_EXAMPLE_SPHERICALCUBE_AVAILABLE = 1;
// This is the Game State for this SphericalCubeExample
// Each entry is a function pointer for Initialize, Update, Destroy
OPgameState GS_EXAMPLE_SPHERICALCUBE = {
	ExampleSphericalCubeEnter,
	ExampleSphericalCubeUpdate,
	ExampleSphericalCubeRender,
	ExampleSphericalCubeExit
};