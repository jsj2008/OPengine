#include "./Examples/Textured.h"
#include "./Human/include/Systems/RenderSystem.h"

#include "./Data/include/ContentManager.h"
#include "./Human/include/Rendering/OPMvertex.h"
#include "./Human/include/Input/Input.h"

OPgameState GS_EXAMPLE_TEXTURED = {
	ExampleTexturedEnter,
	ExampleTexturedUpdate,
	ExampleTexturedExit
};

typedef struct {
	OPmesh* Mesh;
	OPeffect* Effect;
	OPcam* Camera;
	ui32 Rotation;
	OPtexture* Texture;
} TexturedExample;

TexturedExample* texturedExample;

void ExampleTexturedEnter(OPgameState* last) {
	OPcmanLoad("PuzzleBlock.opm");
	OPcmanLoad("TexturedModel.frag");
	OPcmanLoad("TexturedModel.vert");
	OPcmanLoad("TetrisBroken.png");

	texturedExample = (TexturedExample*)OPalloc(sizeof(TexturedExample));

	texturedExample->Mesh = (OPmesh*)OPcmanGet("PuzzleBlock.opm");
	texturedExample->Texture = (OPtexture*)OPcmanGet("TetrisBroken.png");
	texturedExample->Rotation = 0;

	OPshaderAttribute attribs[] = {
		{ "aPosition", GL_FLOAT, 3 },
		{ "aNormal", GL_FLOAT, 3 },
		{ "aUV", GL_FLOAT, 2 }
	};

	texturedExample->Effect = (OPeffect*)OPalloc(sizeof(OPeffect));
	OPshader* vert = (OPshader*)OPcmanGet("TexturedModel.vert");
	OPshader* frag = (OPshader*)OPcmanGet("TexturedModel.frag");
	*texturedExample->Effect = OPrenderCreateEffectStride(
		*vert,
		*frag,
		attribs,
		3,
		"Textured Effect",
		texturedExample->Mesh->VertexSize
		);

	texturedExample->Camera = (OPcam*)OPalloc(sizeof(OPcam));
	*texturedExample->Camera = OPcamProj(
		OPvec3One * 2.0,
		OPvec3Create(0, 0, 0),
		OPvec3Create(0, 1, 0),
		0.1f,
		1000.0f,
		45.0f,
		OPrenderWidth / (f32)OPrenderHeight
		);
}

int ExampleTexturedUpdate(OPtimer* time) {
	OPrenderDepth(1);
	OPrenderClear(0, 0, 0);
	
	if (OPkeyboardIsDown(OPKEY_SPACE)) { texturedExample->Rotation++; }

	OPrenderBindMesh(texturedExample->Mesh);
	OPrenderBindEffect(texturedExample->Effect);

	OPmat4 world, view, proj;
	OPmat4buildRotY(&world, texturedExample->Rotation / 100.0);

	OPcamGetView((*texturedExample->Camera), &view);
	OPcamGetProj((*texturedExample->Camera), &proj);

	OPtextureClearActive();
	ui32 tex = OPtextureBind(texturedExample->Texture);
	OPrenderParami("uColorTexture", tex);

	OPrenderParamMat4("uWorld", &world);
	OPrenderParamMat4("uProj", &proj);
	OPrenderParamMat4("uView", &view);

	OPvec3 light = OPvec3Create(0, 1, 0);
	OPrenderParamVec3("vLightDirection", &light);

	OPrenderMesh();

	OPrenderPresent();
	return false;
}

void ExampleTexturedExit(OPgameState* next) {
	OPrenderUnloadEffect(texturedExample->Effect);
	OPfree(texturedExample->Effect);
	OPfree(texturedExample->Camera);

	OPfree(texturedExample);
}