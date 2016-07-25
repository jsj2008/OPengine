#ifndef OPENGINE_HUMAN_RENDERING_WINDOW
#define OPENGINE_HUMAN_RENDERING_WINDOW

struct OPwindow;
typedef struct OPwindow OPwindow;

#include "./Core/include/OPtypes.h"
#include "./Human/include/Rendering/OpenGL.h"

#include "./Human/include/Rendering/OPmonitor.h"
#include "./Core/include/Assert.h"

struct OPwindow {
	ui32 Width;
	ui32 Height;
	f32 WidthScaled;
	f32 HeightScaled;

#ifdef OPIFEX_DIRECTX_11
	HWND Window;
#else
	GLFWwindow* Window;
#endif

	// TODO: (garrett) not how this is supposed to work, but we'll keep it for now
	GLuint VAO;

	void Init(OPmonitor* monitor, bool fullscreen, bool borderless, const OPchar* title, ui32 width, ui32 height);

	OPint Update();
	void Bind();
	void Focus();
	void SetPosition(i32 x, i32 y);


	inline void Init(const OPchar* title) {
		ASSERT(OPMONITOR_SETUP, "Must call OPmonitorSetup() first");
		Init(NULL, true, true, title, OPMONITOR_PRIMARY.VideoModeCurrent.Width, OPMONITOR_PRIMARY.VideoModeCurrent.Height);
	}
	inline void Init(OPmonitor* monitor, bool fullscreen, bool borderless, const OPchar* title) {
		ASSERT(OPMONITOR_SETUP, "Must call OPmonitorSetup() first");
		Init(monitor, fullscreen, borderless, title, OPMONITOR_PRIMARY.VideoModeCurrent.Width, OPMONITOR_PRIMARY.VideoModeCurrent.Height);
	}
};

extern OPwindow* OPWINDOW_ACTIVE;
#endif
