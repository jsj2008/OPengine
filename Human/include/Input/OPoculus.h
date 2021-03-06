#pragma once

struct OPoculus;
typedef struct OPoculus OPoculus;

#define OVR_OS_WIN32

#ifdef OPIFEX_OPTION_OCULUS
#include "./Math/include/OPvec2.h"
#include "./Math/include/OPvec4.h"

#include "./Human/include/Rendering/OPframeBuffer.h"

#include <OVR_CAPI.h>
#include "OVR_CAPI_GL.h"
#endif

struct OPoculus {
#ifdef OPIFEX_OPTION_OCULUS
	ovrHmd _hmdDevice;
	OPframeBuffer frameBuffer;
	ovrGLTexture ovrTexture[2];
	ovrSession session;
	ovrGraphicsLuid luid;
	ovrEyeRenderDesc eyeRenderDesc;
	ovrHmdDesc desc;
#endif
};

extern OPoculus OculusManager;

int OPoculusStartup();
int OPoculusInitialize();
void OPoculusDestroy();
void OPoculusUpdate();
void OPoculusBegin();
void OPoculusEnd();