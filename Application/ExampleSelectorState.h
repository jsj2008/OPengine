#pragma once

#include "./Pipeline/Engine.h"

extern OPgameState GS_EXAMPLE_SELECTOR;

void ExampleSelectorEnter(OPgameState* last);
int ExampleSelectorUpdate(OPtimer* time);
void ExampleSelectorExit(OPgameState* next);