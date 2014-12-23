#ifndef OPENGINE_PIPELINE_TEXTURE3D
#define OPENGINE_PIPELINE_TEXTURE3D

#include "./Math/include/OPvec3.h"
#include "./Human/include/Rendering/OPtexture.h"
#include "./Human/include/Rendering/OPeffect.h"
#include "./Human/include/Rendering/OPcam.h"

typedef struct {
	OPvec3 Position;
	OPvec3 Rotation;
	OPvec3 Scale;
	OPtexture* Texture;
	OPeffect* Effect;
} OPtexture3D;

OPtexture3D* OPtexture3DCreate(OPtexture* texture, OPeffect* effect);
void OPtexture3DDestroy(OPtexture3D* character);
void OPtexture3DPrepRender(OPtexture3D* tex3d, OPcam* camera);
void OPtexture3DRender(OPtexture3D* tex3d, OPcam* camera);

#endif