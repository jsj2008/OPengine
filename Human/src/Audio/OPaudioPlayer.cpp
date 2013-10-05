#include "./Human/include/Audio/OPaudioPlayer.h"

OPaudioPlayer* OPAUD_CURR_PLAYER;

OPaudioPlayer OPaudPlayerCreate(OPaudioSource* src, OPint sounds, OPint looping){
	OPaudioPlayer player = {
		NULL,
		sounds,
		0
	};

	OPLog("Sounds: %d\n", sounds);
	player.Emitters = (OPaudioEmitter*)OPalloc(sizeof(OPaudioEmitter) * sounds);

	for(OPint i = sounds; i--;){
		OPaudioEmitter* e = &player.Emitters[i];
		e = OPaudCreateEmitter(src, looping, EMITTER_THREADED);
	}

	return player;
}

void OPaudPlayerDestroy(OPaudioPlayer* player){
	OPfree(player->Emitters);
}

void OPaudPlayerPlay(){
	OPaudSafePlay(&OPAUD_CURR_PLAYER->Emitters[OPAUD_CURR_PLAYER->Current++]);
	OPAUD_CURR_PLAYER->Current %= OPAUD_CURR_PLAYER->Count;
}

void OPaudPlayerStop(){
	for(OPint i = OPAUD_CURR_PLAYER->Count; i--;){
		if(OPAUD_CURR_PLAYER->Emitters[i].State != Stopped){
			OPaudSafeStop(&OPAUD_CURR_PLAYER->Emitters[i]);
		}
	}
}

void OPaudPlayerPause(){
	for(OPint i = OPAUD_CURR_PLAYER->Count; i--;){
		if(OPAUD_CURR_PLAYER->Emitters[i].State != Paused){
			OPaudSafePause(&OPAUD_CURR_PLAYER->Emitters[i]);
		}
	}
}

void OPaudPlayerUpdate(void(*Proc)(OPaudioEmitter* emit, OPint length)){
	for(OPint i = OPAUD_CURR_PLAYER->Count; i--;){
		OPaudSafeUpdate(&OPAUD_CURR_PLAYER->Emitters[i], Proc);
	}
}