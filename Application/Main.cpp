#include "ExampleSelectorState.h"

//////////////////////////////////////
// Application Methods
//////////////////////////////////////

void ApplicationInit() {
	OPchar* assetDir = NULL;
#ifdef OPIFEX_REPO
	assetDir = OPIFEX_REPO;
#endif

	OPloadersAddDefault();
	OPcmanInit(assetDir);
	OPrenderInit();
	OPgameStateChange(&GS_EXAMPLE_SELECTOR);
}

int ApplicationUpdate(OPtimer* timer) {
	OPcmanUpdate();
	OPinputSystemUpdate();
	if (OPkeyboardWasReleased(OPKEY_ESCAPE)) return 1;	
	if (OPkeyboardWasReleased(OPKEY_BACKSPACE)) {
		OPgameStateChange(&GS_EXAMPLE_SELECTOR);
	}
	return ActiveState->Update(timer);
}

void ApplicationDestroy() {
	ActiveState->Exit(ActiveState);
}

void ApplicationSetup() {
	OPinitialize = ApplicationInit;
	OPupdate = ApplicationUpdate;
	OPdestroy = ApplicationDestroy;
}


//////////////////////////////////////
// Application Entry Point
//////////////////////////////////////

#ifdef OPIFEX_ANDROID

#include <jni.h>
extern "C" {
	JNIEXPORT void JNICALL Java_com_opifex_GL2JNILib_start(JNIEnv * env, jobject obj);
};
JNIEXPORT void JNICALL Java_com_opifex_GL2JNILib_start(JNIEnv * env, jobject obj) {
	ApplicationSetup();
}

#else

int main(int argc, char** args) {
	ApplicationSetup();
	OPstart();
	OPend();
	return 0;
}

#endif