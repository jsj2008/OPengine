#include "GameManager.h"
#include "./Data/include/OPcontentManager.h"
#include "./Math/include/Matrix4.h"
#include "./Human/include/Resources/Texture/ImagePNG.h"
#include "./Human/include/Utilities/Errors.h"
#include "./Human/include/Rendering/Buffer.h"
#include "./Human/include/Rendering/OPmesh.h"
#include "./Human/include/Rendering/OPmeshPacked.h"
#include "./Human/include/Rendering/OPmeshPacker.h"
#include "./Human/include/Rendering/OPframeBuffer.h"
#include "./Human/include/Rendering/OPeffect.h"
#include "./Human/include/Rendering/Camera.h"
#include "./Human/include/Rendering/OPquad.h"
#include "./Core/include/Log.h"
#include "./Performance/include/OPthread.h"
#include "./Human/include/Input/GamePadSystem.h"

OPfloat vertData[] = {
	 0.5,  0.5, 0,
	   1,  0,   1,
	-0.5,  0.5, 0,
	   1,  0,   0,
	-0.5, -0.5, 0,
	   0,  1,   0,
	 0.5, -0.5, 0,
	   0,  0,   1
};

ui16 indexData[] = {
	0, 2, 3, 
	2, 0, 1
};

OPmeshPacked quad;
OPmeshPacker packer;
OPmesh* plane;
OPeffect tri, post;
OPcam camera;
OPtexture* tex, *spec, *norm;
OPframeBuffer rt;
OPthread thread;

void* LoadTest(void* args){
	// OPcmanLoad("impact.wav");
	// OPcmanLoad("boom.wav");
	// OPcmanLoad("background.ogg");
	//OPthreadStop(NULL);
	return NULL;
}

GameManager::GameManager(int width, int height) 
{
	rotateAmnt = 0;
	OPrenderInit(width, height);

	OPLog("Loading started");
	thread = OPthreadStart(LoadTest, NULL);
	OPthreadJoin(&thread);
	OPLog("Loading done");
	OPLog("Loading kicked off");

	OPcmanLoad("impact.wav");
	OPcmanLoad("boom.wav");
	OPcmanLoad("background.ogg");
	OPcmanLoad("TexturedSpecular.vert");
	OPcmanLoad("TexturedSpecular.frag");
	OPcmanLoad("TexturedScreen.vert");
	OPcmanLoad("Textured.frag");
	OPcmanLoad("BiPlane.opm");
	OPcmanLoad("steamPlaneSkin.png");
	OPcmanLoad("steamPlaneSpec.png");
	OPcmanLoad("noneNorm.png");

	packer = OPmeshPackerCreate();

	OPmeshPackerBind(&packer);
	quad = OPquadCreatePacked();
	OPmeshPackerBuild();

	OPtextureDescription desc = {
		512, 512,
		GL_RGBA, GL_UNSIGNED_BYTE,
		OPtextureLinear, OPtextureLinear,
		OPtextureClamp, OPtextureClamp
	};

	rt = OPframeBufferCreate(desc);

	OPLog("GameManager::GameManager - Ready to load model");
	plane = (OPmesh*)OPcmanGet("BiPlane.opm");
	tex  = (OPtexture*)OPcmanGet("steamPlaneSkin.png");
	spec = (OPtexture*)OPcmanGet("steamPlaneSpec.png");
	norm = (OPtexture*)OPcmanGet("noneNorm.png");

	OPvec3 pos = {0, 5, 15.0f};
	OPvec3 look = {0, 0, 0};
	OPvec3 up = {0, 1, 0};

	camera = OPcamProj(
		pos,
		look,
		up,
		0.1f,
		100.0f,
		90,
		(width / (OPfloat)height)
	);

	{
		OPshaderAttribute attribs[] = {
			{"aPosition",GL_FLOAT,3},
			{"aNormal",GL_FLOAT,3},
			{"aTangent",GL_FLOAT,3},
			{"aUV",GL_FLOAT,2}
		};
		
		OPLog("GameManager::GameManager - Ready to load shaders");
		tri = OPrenderCreateEffect(
			*(OPshader*)OPcmanGet("TexturedSpecular.vert"),
			*(OPshader*)OPcmanGet("TexturedSpecular.frag"),
			attribs,
			4
		);
	}

	{
		OPshaderAttribute attribs[] = {
			{"aPosition",GL_FLOAT,3},
			{"aUV",GL_FLOAT,2}
		};
		
		OPLog("GameManager::GameManager - Ready to load shaders");
		post = OPrenderCreateEffect(
			*(OPshader*)OPcmanGet("TexturedScreen.vert"),
			*(OPshader*)OPcmanGet("Textured.frag"),
			attribs,
			2
		);
	}

	OPLog("GameManager:Constructor Finished");
}

OPfloat t = 0;
bool GameManager::Update( OPtimer* coreTimer )
{
	t += 0.005f * coreTimer->Elapsed;
	return true;
}


void GameManager::Draw(){
	//OPrenderClear(0.3f, 0.3f, 0.5f);
	OPmat4 world, view, proj;
	world = OPmat4();
	view = OPmat4();
	proj = OPmat4();

	OPmat4buildRotY(&world, t);
	OPcamGetView(camera, &view);
	OPcamGetProj(camera, &proj);

	OPmeshPackerBind(&packer);
	OPrenderBindMesh(plane);
	OPrenderBindEffect(&tri);

	OPtextureBind(tex);
	OPrenderParami("uColorTexture", tex->Handle);
	OPtextureBind(spec);
	OPrenderParami("uSpecularTexture", spec->Handle);
	OPtextureBind(norm);
	OPrenderParami("uNormalTexture", norm->Handle);
	OPrenderParamMat4v("uWorld", 1, &world);
	OPrenderParamMat4v("uProj", 1, &proj);
	OPrenderParamMat4v("uView", 1, &view);

	OPframeBufferBind(&rt);
	
	GamePadController* gamePad = OPgamePadController(GamePadIndex_One);
	OPgamePadUpdate(gamePad);
	
	if(OPgamePadIsConnected(gamePad)) {
		if(OPgamePadIsDown(gamePad, GamePad_Button_A) || OPgamePadIsDown(gamePad, GamePad_Button_B) || OPgamePadIsDown(gamePad, GamePad_Button_X) || OPgamePadIsDown(gamePad, GamePad_Button_Y)) {
			OPrenderClear( 0.0f, 0.0f, 1.0f);
		} else {
			OPrenderClear( 0.0f, 0.0f, 0.0f);
		}
	} else {
		OPrenderClear(1.0f, 1.0f, 1.0f);
	}

	OPrenderMesh();
	OPframeBufferUnbind();
	
		OPrenderClear(1.0f, 1.0f, 1.0f);
	OPrenderSetViewport(0, 0, OPrenderWidth, OPrenderHeight);

	OPmeshPackerBind(&packer);
	OPrenderBindEffect(&post);
	OPmat4identity(&world);
	OPrenderParamMat4v("uWorld", 1, &world);

	OPtextureBind(&rt.Texture);
	OPrenderParami("uTexture", rt.Texture.Handle);

	OPrenderMeshPacked(&quad);
}