#pragma once

#include "./Data/include/GameStates.h"

extern OPgameState GS_EXAMPLE_SPINE;

void ExampleSpineEnter(OPgameState* last);
int ExampleSpineUpdate(OPtimer* time);
void ExampleSpineExit(OPgameState* next);