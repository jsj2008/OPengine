#include "./ExampleSelectorState.h"
#include "./Human/include/Systems/OPrenderSystem.h"

#include "./Data/include/OPcman.h"
#include "./Human/include/Rendering/OPMvertex.h"
#include "./Human/include/Rendering/Skinning/OPskeleton.h"

#include "./Human/include/Systems/OPinputSystem.h"

void ExampleSkinningEnter(OPgameState* last);
OPint ExampleSkinningUpdate(OPtimer* time);
void ExampleSkinningExit(OPgameState* next);

OPgameState GS_EXAMPLE_SKINNING = {
	ExampleSkinningEnter,
	ExampleSkinningUpdate,
	ExampleSkinningExit
};

typedef struct {

	OPmesh* Mesh;
	OPeffect* Effect;
	OPcam* Camera;
	ui32 pos;
	OPtexture* texture;
} SkinningExample;

SkinningExample* skinningExample;

void ExampleSkinningEnter(OPgameState* last) {
	OPcmanLoad("skinned2.opm");
	OPcmanLoad("Skinning.frag");
	OPcmanLoad("Skinning.vert");
	OPcmanLoad("Knight.png");
	skinningExample = (SkinningExample*)OPalloc(sizeof(SkinningExample));


	skinningExample->texture = (OPtexture*)OPcmanGet("Knight.png");

	skinningExample->pos = 0;
	skinningExample->Mesh = (OPmesh*)OPcmanGet("skinned2.opm");

	OPshaderAttribute attribs[] = {
		{ "aPosition", GL_FLOAT, 3 },
		{ "aNormal", GL_FLOAT, 3 },
		{ "aTangent", GL_FLOAT, 3 },
		//{ "aUV", GL_FLOAT, 2 },
		{ "aBlendIndices", GL_FLOAT, 4 },
		{ "aBlendWeights", GL_FLOAT, 4 }
	};

	skinningExample->Effect = (OPeffect*)OPalloc(sizeof(OPeffect));
	OPshader* vert = (OPshader*)OPcmanGet("Skinning.vert");
	OPshader* frag = (OPshader*)OPcmanGet("Skinning.frag");
	*skinningExample->Effect = OPrenderCreateEffectStride(
		*vert,
		*frag,
		attribs,
		5,
		"Model Effect",
		skinningExample->Mesh->VertexSize
		);

	skinningExample->Camera = (OPcam*)OPalloc(sizeof(OPcam));
	f32 pos = 10;
	*skinningExample->Camera = OPcamProj(
		OPvec3Create(pos, pos, pos),
		OPvec3Create(0, 0, 0),
		OPvec3Create(0, 1, 0),
		0.1f,
		5000.0f,
		45.0f,
		OPRENDER_WIDTH / (f32)OPRENDER_HEIGHT
		);
}

OPint ExampleSkinningUpdate(OPtimer* time) {
	OPrenderDepth(1);
	OPrenderClear(0, 0, 0);

	if (OPkeyboardWasPressed(OPKEY_P)) { skinningExample->pos++; }
	if (OPkeyboardWasPressed(OPKEY_O)) { skinningExample->pos--; }

	if (OPkeyboardIsDown(OPKEY_UP)) { skinningExample->Camera->_pos.y += 1.1; }
	if (OPkeyboardIsDown(OPKEY_DOWN)) { skinningExample->Camera->_pos.y -= 1.1; }
	if (OPkeyboardIsDown(OPKEY_LEFT)) { skinningExample->Camera->_pos.x -= 1.1; }
	if (OPkeyboardIsDown(OPKEY_RIGHT)) { skinningExample->Camera->_pos.x += 1.1; }

	skinningExample->Camera->_viewStale = 1;
	OPcamUpdateView((*skinningExample->Camera));

	//OPmat4Translate(&mesh->Skeleton->localPoses[pos], time->Elapsed / 1000.0f, 0, 0);
	OPmat4RotZ(&skinningExample->Mesh->Skeleton->localPoses[skinningExample->pos], OPkeyboardIsDown(OPKEY_W) / 10.0f);
	OPmat4RotZ(&skinningExample->Mesh->Skeleton->localPoses[skinningExample->pos], OPkeyboardIsDown(OPKEY_S) / -10.0f);
	OPskeletonUpdate(skinningExample->Mesh->Skeleton);

	OPrenderBindMesh(skinningExample->Mesh);
	OPrenderBindEffect(skinningExample->Effect);
	
	OPmat4 world, view, proj;
	OPmat4Identity(&world);
	//OPmat4BuildRotX(&world,- OPpi / 2.0);

	OPcamGetView((*skinningExample->Camera), &view);
	OPcamGetProj((*skinningExample->Camera), &proj);

	OPrenderParamMat4("uWorld", &world);
	OPrenderParamMat4("uView", &view);
	OPrenderParamMat4("uProj", &proj);

	OPrenderParamMat4v("uBones", skinningExample->Mesh->Skeleton->hierarchyCount, skinningExample->Mesh->Skeleton->skinned);

	OPvec3 light = OPvec3Create(0, 10, 0);
	OPrenderParamVec3("uLightPosition", &light);

	OPtextureClearActive();
	OPrenderParami("uColorTexture", OPtextureBind(skinningExample->texture));

	OPrenderMesh();

	OPrenderPresent();
	return false;
}

void ExampleSkinningExit(OPgameState* next) {
	OPrenderUnloadEffect(skinningExample->Effect);
	OPfree(skinningExample->Effect);
	OPfree(skinningExample->Camera);
}