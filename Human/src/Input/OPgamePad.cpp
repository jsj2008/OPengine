#include "./Human/include/Input/OPgamePad.h"
#include "./Core/include/OPtypes.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/OPlog.h"
#include "GLFW/glfw3.h"

#if !defined(OPIFEX_ANDROID) && !defined(OPIFEX_IOS) && defined(OPIFEX_UNIX)
	//#include <GLFW/glfw3.h>
#elif defined(OPIFEX_WINDOWS)
	#include <Xinput.h>
	#pragma comment(lib, "XInput.lib")
#elif defined(OPIFEX_ANDROID)
	#include "./Core/include/OPcore.h"
	#include "./Human/include/Utilities/AndroidNDK.h"
#endif

#ifdef OPIFEX_ANDROID

	#define CONTROLLER_CLASS_NAME "tv/ouya/console/api/OuyaController"

	jobject OPjniGetControllerByPlayer(OPint playerNum) {
		OPJniMethodInfo methodInfo;
		if (!OPjniGetStaticMethodInfo(methodInfo,
			CONTROLLER_CLASS_NAME, "getControllerByPlayer",
			"(I)Ltv/ouya/console/api/OuyaController;")) {
				return 0;
		}
		return methodInfo.env->CallStaticObjectMethod(methodInfo.classID,
			methodInfo.methodID, playerNum);
	}


	OPint OPjniGetControllerButton(jobject controller, OPint button) {

		OPJniMethodInfo methodInfo2;
		if (!OPjniGetMethodInfo(methodInfo2,
			CONTROLLER_CLASS_NAME, "getButton", "(I)Z")) {
				OPlog("Class method not found");
				return false;
		}

		JNIEnvironment()->DeleteLocalRef(methodInfo2.classID);

		OPint result = JNIEnvironment()->CallBooleanMethod(controller, methodInfo2.methodID, button);

		if(result) {
			OPlog("Button Pressed: %d", button);
		} else {

		}

		return result;
	}

	OPfloat OPjniGetAxisValue( jobject controller, OPint ouyaAxis ) {

		OPJniMethodInfo methodInfo;

		if (!OPjniGetMethodInfo(methodInfo, CONTROLLER_CLASS_NAME, "getAxisValue", "(I)F"))
		{
			return 0;
		}

		jfloat f = methodInfo.env->CallFloatMethod(controller, methodInfo.methodID, ouyaAxis);

		methodInfo.env->DeleteLocalRef(methodInfo.classID);

		return f;
	}

#endif


#ifdef OPIFEX_ANDROID
void __OPandUpdateGamePad(OPgamePad* controller){
	jobject jcontroller = OPjniGetControllerByPlayer(controller->playerIndex);

	if(jcontroller == 0) {
		return;
	}

	controller->connected = true;

	controller->buttons[OPgamePadButton::A] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_O);
	controller->buttons[OPGAMEPADBUTTON_X] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_U);
	controller->buttons[OPgamePadButton::B] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_A);
	controller->buttons[OPGAMEPADBUTTON_Y] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_Y);

	controller->buttons[OPGAMEPADBUTTON_LEFT_SHOULDER] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_L1);
	controller->buttons[OPGAMEPADBUTTON_RIGHT_SHOULDER] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_R1);

	controller->buttons[OPGAMEPADBUTTON_LEFT_THUMB] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_L3);
	controller->buttons[OPGAMEPADBUTTON_RIGHT_THUMB] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_R3);


	controller->buttons[OPgamePadButton::DPAD_UP] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_DPAD_UP);
	controller->buttons[OPgamePadButton::DPAD_DOWN] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_DPAD_DOWN);
	controller->buttons[OPGAMEPADBUTTON_DPAD_LEFT] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_DPAD_LEFT);
	controller->buttons[OPgamePadButton::DPAD_RIGHT] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_DPAD_RIGHT);

	controller->buttons[OPGAMEPADBUTTON_START] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_MENU);

	controller->axes[OPGAMEPADAXIS_L2] = OPjniGetAxisValue(jcontroller, OUYA_AXIS_L2);
	controller->axes[OPGAMEPADAXIS_R2] = OPjniGetAxisValue(jcontroller, OUYA_AXIS_R2);

	controller->axes[OPGAMEPADAXIS_LS_X] = OPjniGetAxisValue(jcontroller, OUYA_AXIS_LS_X);
	controller->axes[OPGAMEPADAXIS_LS_Y] = -OPjniGetAxisValue(jcontroller, OUYA_AXIS_LS_Y);

	controller->axes[OPGAMEPADAXIS_RS_X] = OPjniGetAxisValue(jcontroller, OUYA_AXIS_RS_X);
	controller->axes[OPGAMEPADAXIS_RS_Y] = -OPjniGetAxisValue(jcontroller, OUYA_AXIS_RS_Y);
}
#endif

void UpdateGamePadWithGLFW(OPgamePad* c) {

	i32  axes = 0, buttons = 0;
	const f32* axisData = NULL;
	const ui8* buttonData = NULL;

	// make sure the gamepad is connected

	if(!glfwJoystickPresent((i32)c->controllerIndex)){
		//OPlog("Controller %d not connected", (i32)c->playerIndex);
		c->connected = 0;
		return;
	}

	c->connected = true;
	const i8* name = glfwGetJoystickName(GLFW_JOYSTICK_1 + c->controllerIndex);
	OPlog("Controller: %s", name);

	// get axis data, andmake sure the number of axes are expected

	axisData = glfwGetJoystickAxes(c->controllerIndex, &axes);


	OPlog("Axes %d", axes);
	 //if(axes != 8){
	 //	// game pads should have 8 axes
	 //	// 2 for each stick, and 2 for the dpad
	 //	c->connected = 0;
	 //	return;
	 //}

	// map left stick and trigger
	c->axes[(ui32)OPgamePadAxis::LS_X] = axisData[0];
	c->axes[(ui32)OPgamePadAxis::LS_Y] = -axisData[1];

	c->axes[(ui32)OPgamePadAxis::RS_X] = axisData[2];
	c->axes[(ui32)OPgamePadAxis::RS_Y] = -axisData[3];

	c->axes[(ui32)OPgamePadAxis::L2] = (axisData[4] + 1.0f) / 2.0f;
	c->axes[(ui32)OPgamePadAxis::R2] = (axisData[5] + 1.0f) / 2.0f;

	// OPlog("L %f %f %f", c->axes[OPGAMEPADAXIS_LS_X], c->axes[OPGAMEPADAXIS_LS_Y], c->axes[OPGAMEPADAXIS_L2]);
	// OPlog("R %f %f %f", c->axes[OPGAMEPADAXIS_RS_X], c->axes[OPGAMEPADAXIS_RS_Y], c->axes[OPGAMEPADAXIS_R2]);

	 if(axes == 8) {
	 	// map right stick and trigger
	 	c->axes[(ui32)OPgamePadAxis::RS_X] =  axisData[3];
	 	c->axes[(ui32)OPgamePadAxis::RS_Y] = -axisData[4];
	 	c->axes[(ui32)OPgamePadAxis::R2]   =  (axisData[5] + 1.0f) / 2.0f;
	 }


	// get button data, make sure it's all kosher

	 buttonData = glfwGetJoystickButtons(c->controllerIndex, &buttons);
	if (buttons < 10) {
		// game pads should have 10 buttons
		OPlog("GamePad had %d buttons but has to have at least %d buttons", buttons, 10);
		c->connected = 0;
		return;
	}

	c->buttons[(ui32)OPgamePadButton::DPAD_LEFT] = buttonData[2];
	c->buttons[(ui32)OPgamePadButton::DPAD_RIGHT] = buttonData[3];
	c->buttons[(ui32)OPgamePadButton::DPAD_UP] = buttonData[0];
	c->buttons[(ui32)OPgamePadButton::DPAD_DOWN] = buttonData[1];
	// map buttons
	c->buttons[(ui32)OPgamePadButton::A] = buttonData[11];
	c->buttons[(ui32)OPgamePadButton::B] = buttonData[12];
	c->buttons[(ui32)OPgamePadButton::X] = buttonData[13];
	c->buttons[(ui32)OPgamePadButton::Y] = buttonData[14];
	c->buttons[(ui32)OPgamePadButton::LEFT_SHOULDER] = buttonData[8];
	c->buttons[(ui32)OPgamePadButton::RIGHT_SHOULDER] = buttonData[9];
	c->buttons[(ui32)OPgamePadButton::BACK] = buttonData[5];
	c->buttons[(ui32)OPgamePadButton::START] = buttonData[4];
	c->buttons[(ui32)OPgamePadButton::LEFT_THUMB] = buttonData[6];
	c->buttons[(ui32)OPgamePadButton::RIGHT_THUMB] = buttonData[7];

}

#if defined(OPIFEX_UNIX) && !defined(OPIFEX_IOS)
void __OPlnxUpdateGamePad(OPgamePad* c){

	UpdateGamePadWithGLFW(c);
	return;
}
#endif
//-----------------------------------------------------------------------------
#ifdef OPIFEX_WINDOWS
void __OPwinUpdateGamePad(OPgamePad* controller){

	//UpdateGamePadWithGLFW(controller);
	//return;

	XINPUT_STATE controllerState;
	ZeroMemory(&controllerState, sizeof(XINPUT_STATE));

	DWORD result = XInputGetState((DWORD)controller->controllerIndex, &controllerState);

	//bool glfwSays = glfwJoystickPresent(controller->controllerIndex);

	// check to see if the controller is connected
	if(result == ERROR_SUCCESS){
		// if it's connected, get the state
		controller->connected = true;

		// check other states
#pragma region Trigger and stick states

		controller->axes[(ui32)OPgamePadAxis::L2] = controllerState.Gamepad.bLeftTrigger / 255.0f;
		controller->axes[(ui32)OPgamePadAxis::R2] = controllerState.Gamepad.bRightTrigger / 255.0f;

		controller->axes[(ui32)OPgamePadAxis::LS_X] = controllerState.Gamepad.sThumbLX / (OPfloat)SHRT_MAX;
		controller->axes[(ui32)OPgamePadAxis::LS_Y] = controllerState.Gamepad.sThumbLY / (OPfloat)SHRT_MAX;

		controller->axes[(ui32)OPgamePadAxis::RS_X] = controllerState.Gamepad.sThumbRX / (OPfloat)SHRT_MAX;
		controller->axes[(ui32)OPgamePadAxis::RS_Y] = controllerState.Gamepad.sThumbRY / (OPfloat)SHRT_MAX;
#pragma endregion

#pragma region Button states

		controller->buttons[(ui32)OPgamePadButton::LEFT_SHOULDER] =
			(controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) > 0;

		controller->buttons[(ui32)OPgamePadButton::RIGHT_SHOULDER] =
			(controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) > 0;

		controller->buttons[(ui32)OPgamePadButton::A] =
			(controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_A) > 0;

		controller->buttons[(ui32)OPgamePadButton::B] =
			(controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_B) > 0;

		controller->buttons[(ui32)OPgamePadButton::X] =
			(controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_X) > 0;

		controller->buttons[(ui32)OPgamePadButton::Y] =
			(controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) > 0;

		controller->buttons[(ui32)OPgamePadButton::DPAD_UP] =
			(controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) > 0;

		controller->buttons[(ui32)OPgamePadButton::DPAD_DOWN] =
			(controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) > 0;

		controller->buttons[(ui32)OPgamePadButton::DPAD_LEFT] =
			(controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) > 0;

		controller->buttons[(ui32)OPgamePadButton::DPAD_RIGHT] =
			(controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) > 0;

		controller->buttons[(ui32)OPgamePadButton::START] =
			(controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_START) > 0;

		controller->buttons[(ui32)OPgamePadButton::BACK] =
			(controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) > 0;

		controller->buttons[(ui32)OPgamePadButton::LEFT_THUMB] =
			(controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) > 0;

		controller->buttons[(ui32)OPgamePadButton::RIGHT_THUMB] =
			(controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) > 0;
#pragma endregion

	}
	else {
		controller->connected = false;
	}
}
#endif

void OPgamePad::Update(OPtimer* timer){

	if (rumbleTime > 0) {

		rumbleTime -= timer->Elapsed;
		if (rumbleTime <= 0) {
			rumble[0] = 0;
			rumble[1] = 0;
		}
        #ifdef OPIFEX_WINDOWS
		XINPUT_VIBRATION vibration;
		vibration.wLeftMotorSpeed = rumble[0] * 65535;
		vibration.wRightMotorSpeed = rumble[1] * 65535;
		XInputSetState((DWORD)controllerIndex, &vibration);
        #endif
	}


	if(connected) {
		OPmemcpy(
			&prevButtons,
			&buttons,
			sizeof(bool) * (ui32)OPgamePadButton::_MAX);
		OPmemcpy(
			&prevAxes,
			&axes,
			sizeof(OPfloat) * (ui32)OPgamePadAxis::_MAX);
	}

#ifdef OPIFEX_ANDROID
	__OPandUpdateGamePad(controller);
#endif

#if defined(OPIFEX_UNIX) && !defined(OPIFEX_IOS)
	__OPlnxUpdateGamePad(this);
#endif

#ifdef OPIFEX_WINDOWS
	__OPwinUpdateGamePad(this);
#endif

	OPvec2 axis;
	OPfloat len;

	axis.x = axes[0];
	axis.y = axes[1];
	len = OPvec2Len(axis);
	axes[0] *= len > deadzone;
	axes[1] *= len > deadzone;

	axis.x = axes[2];
	axis.y = axes[3];
	len = OPvec2Len(axis);
	axes[2] *= len > deadzone;
	axes[3] *= len > deadzone;

	axis.x = axes[4];
	axis.y = axes[5];
	len = OPvec2Len(axis);
	axes[4] *= len > deadzone;
	axes[5] *= len > deadzone;
}

void OPgamePad::SetRumble(OPfloat left, OPfloat right, i64 time) {
	rumble[0] = left;
	rumble[1] = right;
	rumbleTime = time;
}

void OPgamePad::Reset(){
	OPbzero(&buttons, sizeof(bool) * (ui32)OPgamePadButton::_MAX);
	OPbzero(&prevButtons, sizeof(bool) * (ui32)OPgamePadButton::_MAX);
	OPbzero(&axes, sizeof(OPfloat) * (ui32)OPgamePadAxis::_MAX);
}

bool OPgamePad::AnyPrevInputIsDown() {
	ui32 count = (ui32)OPgamePadButton::_MAX;
	for (; count > 0; count--) {
		if (prevButtons[count]) return true;
	}
	return false;
}

bool OPgamePad::AnyInputIsDown() {
	ui32 count = (ui32)OPgamePadButton::_MAX;
	for (; count > 0; count--) {
		if (buttons[count]) return true;
	}
	return false;
}

OPvec2 OPgamePad::LeftThumb() {
	return OPvec2(axes[(ui32)OPgamePadAxis::LS_X], axes[(ui32)OPgamePadAxis::LS_Y]);
}

OPvec2 OPgamePad::RightThumb() {
	return OPvec2(axes[(ui32)OPgamePadAxis::RS_X], axes[(ui32)OPgamePadAxis::RS_Y]);
}
