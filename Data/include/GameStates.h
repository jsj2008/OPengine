#ifndef OPEngine_Data_GameStates
#define OPEngine_Data_GameStates
#include "./Core/include/Timer.h"
#include "./Core/include/Types.h"
#include "List.h"

#ifdef __cplusplus
extern "C" {
#endif
struct OPgameState_def;
typedef struct OPgameState_def OPgameState;

struct OPgameState_def{
	void (*Init)(OPgameState*);
	int(*Update)(OPtimer*);
	void (*Destroy)(OPgameState*);
	void* Data;
};

extern OPgameState* ActiveState;

void OPgameStateChange(OPgameState* targetState); 
OPgameState* OPgameStateCreate(void (*entrance)(OPgameState*), int(*update)(OPtimer*), void (*exit)(OPgameState*));
OPint OPgameStateDestroy(OPgameState* state);

#ifdef __cplusplus
}
#endif
#endif
