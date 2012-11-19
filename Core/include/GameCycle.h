#pragma once
#ifndef OPEngine_Core_GameCycle
#define OPEngine_Core_GameCycle
#include "Timer.h"

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif
extern void (*OPinitialize)();
//----------------------------------------------------------------------------
extern void (*OPupdate)(OPtimer*);
//----------------------------------------------------------------------------
extern void (*OPdestroy)();
#ifdef __cplusplus
};
#endif
#endif
