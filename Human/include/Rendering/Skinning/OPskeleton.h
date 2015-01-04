#ifndef OPIFEX_RENDERING_SKINNING_ANIMATION
#define OPIFEX_RENDERING_SKINNING_ANIMATION

#define NUM_JOINTS 4

#include "./Math/include/OPmat4.h"
#include "./Math/include/OPvec4.h"

typedef struct {
	i16* hierarchy;
	ui16 hierarchyCount;
	OPmat4* bindPoses;
	OPmat4* invBindPoses;
	OPmat4* globalPoses;
	OPmat4* globalInvPoses;
	OPmat4* localPoses;
	OPmat4* skinned;
} OPskeleton;

OPskeleton* OPskeletonCreate(i16* hierarchy, OPmat4* pose, i32 count);
void OPskeletonUpdate(OPskeleton* skeleton);

#endif