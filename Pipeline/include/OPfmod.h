#ifndef OPENGINE_PIPELINE_FMOD
#define OPENGINE_PIPELINE_FMOD

#include "./Core/include/OPtypes.h"

#ifdef OPIFEX_OPTION_FMOD
#include "./fmod.hpp"

typedef FMOD::ChannelGroup OPfmodChannelGroup;
typedef FMOD::System OPfmod;
typedef FMOD::Sound OPfmodSound;
typedef FMOD::Channel OPfmodChannel;

extern OPfmod* CURRENT_FMOD;

void OPfmodInit();
OPfmodSound* OPfmodLoad(OPchar* name);
OPfmodChannel* OPfmodPlay(OPfmodSound* sound);
OPfmodChannelGroup* OPfmodCreateChannelGroup(const OPchar* name);
OPfmodChannel* OPfmodPlay(OPfmodSound* sound, OPfmodChannelGroup* group);
void OPfmodSetVolume(OPfmodChannel* channel, OPfloat volume);
void OPfmodSetVolume(OPfmodChannelGroup* channel, OPfloat volume);
void OPfmodUpdate();

inline OPint OPfmodIsPlaying(OPfmodChannel* channel) {
	bool isPlaying;
	channel->isPlaying(&isPlaying);
	return isPlaying;
}

inline void OPfmodSetPause(OPfmodChannel* channel, i8 pause) {
	channel->setPaused(pause);
}

#endif

#endif