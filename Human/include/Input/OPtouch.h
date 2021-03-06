#pragma once

struct OPtouchState;
typedef struct OPtouchState OPtouchState;

#include "./Core/include/OPtypes.h"

struct OPtouchState {
	OPint tapping;
	OPint prevTapping;
	OPint updated;
	f32	positionX;
	f32	positionY;
	f32	prevPositionX;
	f32	prevPositionY;
};

extern OPtouchState Touch;

void OPtouchUpdate();
f32 OPtouchPositionX();
f32 OPtouchPositionY();
f32 OPtouchPositionMovedX();
f32 OPtouchPositionMovedY();
OPint OPtouchIsDown();
OPint OPtouchIsUp();
OPint OPtouchWasPressed();
OPint OPtouchWasReleased();
OPint OPtouchAnyInputIsDown();