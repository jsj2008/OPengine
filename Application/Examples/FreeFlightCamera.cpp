#include "./ExampleSelectorState.h"
#include "./OPengine.h"

typedef struct {
	OPmesh* Mesh;
	OPeffect* Effect;
	OPcamFreeFlight Camera;
	ui32 Rotation;
	OPtexture* Texture;
} FreeFlightExample;

FreeFlightExample freeFlightExample;

void ExampleFreeFlightEnter(OPgameState* last) {
	OPcmanLoad("PuzzleBlock.opm");
	OPcmanLoad("Common/Texture.frag");
	OPcmanLoad("Common/Texture3D.vert");
	OPcmanLoad("TetrisBroken.png");

	freeFlightExample.Mesh = (OPmesh*)OPcmanGet("PuzzleBlock.opm");
	freeFlightExample.Texture = (OPtexture*)OPcmanGet("TetrisBroken.png");
	freeFlightExample.Rotation = 0;

	OPshaderAttribute attribs[] = {
		{ "aPosition", GL_FLOAT, 3 },
		{ "aNormal", GL_FLOAT, 3 },
		{ "aUV", GL_FLOAT, 2 }
	};

	freeFlightExample.Effect = (OPeffect*)OPalloc(sizeof(OPeffect));
	OPshader* vert = (OPshader*)OPcmanGet("Common/Texture3D.vert");
	OPshader* frag = (OPshader*)OPcmanGet("Common/Texture.frag");
	*freeFlightExample.Effect = OPeffectCreate(
		*vert,
		*frag,
		attribs,
		3,
		"Textured Effect",
		freeFlightExample.Mesh->VertexSize
		);

	OPcamFreeFlightInit(&freeFlightExample.Camera, 3.0f, 3.0f, OPVEC3_ONE);
}

OPint ExampleFreeFlightUpdate(OPtimer* timer) {
	OPcamFreeFlightUpdate(&freeFlightExample.Camera, timer);

	OPrenderDepth(1);
	OPrenderClear(0, 0, 0);

	if (OPkeyboardIsDown(OPKEY_SPACE)) { freeFlightExample.Rotation++; }

	OPmeshBind(freeFlightExample.Mesh);
	OPeffectBind(freeFlightExample.Effect);

	OPmat4 world, view, proj;
	world = OPmat4RotY(freeFlightExample.Rotation / 100.0);

	OPcamGetView((freeFlightExample.Camera.Camera), &view);
	OPcamGetProj((freeFlightExample.Camera.Camera), &proj);

	OPtextureClearActive();
	ui32 tex = OPtextureBind(freeFlightExample.Texture);
	OPeffectParami("uColorTexture", tex);

	OPeffectParamMat4("uWorld", &world);
	OPeffectParamMat4("uProj", &proj);
	OPeffectParamMat4("uView", &view);

	OPvec3 light = OPvec3Create(0, 1, 0);
	OPeffectParamVec3("vLightDirection", &light);

	OPmeshRender();

	OPrenderPresent();
	return false;
}

OPint ExampleFreeFlightExit(OPgameState* next) {
	OPeffectUnload(freeFlightExample.Effect);
	OPfree(freeFlightExample.Effect);

	return 0;
}

OPint GS_EXAMPLE_FREEFLIGHT_AVAILABLE = 1;
OPgameState GS_EXAMPLE_FREEFLIGHT = {
	ExampleFreeFlightEnter,
	ExampleFreeFlightUpdate,
	ExampleFreeFlightExit
};