#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./Pipeline/include/Rendering.h"
#include "./Pipeline/include/OPmaterialPBR.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Data/include/OPcman.h"

typedef struct {
	OPfloat Rotation;
	OPscene scene;
	OPrendererForward* renderer;
	OPmodel model;
	OPmodel model2;
	OPcamFreeFlight camera;
	OPmaterialPBR materialPBR;
	OPtextureCube environment;
	OPmaterialPBRInstance* materialInstance;
} SceneExample;

SceneExample sceneExample;

void ExampleSceneEnter(OPgameState* last) {
	const OPchar* envImages[6] = {
		"Textures/TetrisBroken.png",
		"Textures/TetrisYellow.png",
		"Textures/TetrisGreen.png",
		"Textures/TetrisGray.png",
		"Textures/TetrisOrange.png",
		"Textures/TetrisRed.png"
	};
	sceneExample.environment.Init(envImages);

	sceneExample.renderer = OPNEW(OPrendererForward());
	sceneExample.scene.Init(&sceneExample.renderer->rendererRoot, 100);
	sceneExample.camera.Init();
	sceneExample.scene.camera = &sceneExample.camera.Camera;

	sceneExample.materialPBR.Init(OPNEW(OPeffect("Common/PBR.vert", "Common/PBR.frag")));
	sceneExample.materialPBR.rootMaterial.AddParam("uCamPos", &sceneExample.camera.Camera.pos);
	sceneExample.renderer->SetMaterial(&sceneExample.materialPBR.rootMaterial, 0);

	sceneExample.materialInstance = sceneExample.materialPBR.CreateInstance();
	sceneExample.materialInstance->SetAlbedoMap("Dagger_Albedo.png");
	sceneExample.materialInstance->SetSpecularMap("Dagger_Specular.png");
	sceneExample.materialInstance->SetGlossMap("Dagger_Gloss.png");
	sceneExample.materialInstance->SetNormalMap("Dagger_Normals.png");
	sceneExample.materialInstance->SetEnvironmentMap(&sceneExample.environment);

	sceneExample.model.Init("daggerpbr.opm");
	sceneExample.model2.Init("daggerpbr.opm");

	sceneExample.scene.Add(&sceneExample.model, &sceneExample.materialInstance->rootMaterialInstance);
	sceneExample.scene.Add(&sceneExample.model2, &sceneExample.materialInstance->rootMaterialInstance);
}

OPint ExampleSceneUpdate(OPtimer* time) {
	sceneExample.camera.Update(time);
	sceneExample.Rotation += time->Elapsed * 0.25f;
	sceneExample.model.world.SetRotY(sceneExample.Rotation / 200.0f)->Scl(1.0f);
	sceneExample.model2.world.SetScl(1.0f)->Translate(-45, 0, 0);
	return false;
}

void ExampleSceneRender(OPfloat delta) {
	OPrenderClear(0.2f, 0.2f, 0.2f);
	sceneExample.scene.Render(delta);
	OPrenderPresent();
}

OPint ExampleSceneExit(OPgameState* next) {
	OPfree(sceneExample.materialInstance);
	sceneExample.materialPBR.rootMaterial.effect->Destroy();
	OPfree(sceneExample.materialPBR.rootMaterial.effect);
	sceneExample.scene.Destroy();
	sceneExample.renderer->Destroy();
	OPfree(sceneExample.renderer);
	sceneExample.environment.Destroy();

	return 0;
}

OPint GS_EXAMPLE_SCENE_AVAILABLE = 1;
OPgameState GS_EXAMPLE_SCENE = {
	ExampleSceneEnter,
	ExampleSceneUpdate,
	ExampleSceneRender,
	ExampleSceneExit
};
