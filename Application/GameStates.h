#pragma once

#include "./Data/include/OPgameState.h"

extern OPgameState State0;
extern OPgameState State1;

void State0Enter(OPgameState* last);
OPint State0Update(OPtimer* time);
void State0Render(OPfloat delta);
OPint State0Exit(OPgameState* next);


void State1Enter(OPgameState* last);
OPint State1Update(OPtimer* time);
void State1Render(OPfloat delta);
OPint State1Exit(OPgameState* next);