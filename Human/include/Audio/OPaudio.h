#pragma once

#include "./Core/include/OPtypes.h"


#include "./Core/include/OPmath.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/OPlog.h"
#include "./Data/include/OPfile.h"
#include "./Math/include/OPvec3.h"
#include "./Performance/include/OPthread.h"
#include "./Performance/include/OPmutex.h"

#ifdef OPIFEX_OPTION_AUDIO
#ifdef OPIFEX_ANDROID
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#elif defined(OPIFEX_WINDOWS)
#include <AL/al.h>
#include <AL/alc.h>
#include <WTypes.h>
#elif defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_OSX32) || defined(OPIFEX_OSX64)
#include <string.h>
#include <AL/al.h>
#include <AL/alc.h>
#endif

#include <ogg/ogg.h>
#ifdef OPIFEX_ANDROID
#include "./External/Vorbis/include/vorbis/vorbisfile.h"
//#include "./Human/include/Utilities/ogg/ogg.h"
#else
#include <vorbis/vorbisfile.h>
//#include <ogg/ogg.h>
#endif
#endif



//-----------------------------------------------------------------------------
//           _____ _____  __      __        _       _     _           
//     /\   |  __ \_   _| \ \    / /       (_)     | |   | |          
//    /  \  | |__) || |    \ \  / /_ _ _ __ _  __ _| |__ | | ___  ___ 
//   / /\ \ |  ___/ | |     \ \/ / _` | '__| |/ _` | '_ \| |/ _ \/ __|
//  / ____ \| |    _| |_     \  / (_| | |  | | (_| | |_) | |  __/\__ \
// /_/    \_\_|   |_____|     \/ \__,_|_|  |_|\__,_|_.__/|_|\___||___/

#ifdef OPIFEX_OPTION_AUDIO
#ifdef OPIFEX_ANDROID
#define AL_FORMAT_MONO16 0
#define AL_FORMAT_STEREO16 1

extern SLObjectItf SLES_engineObject;
extern SLEngineItf SLES_engineEngine;
extern SLObjectItf SLES_outputMixObject;
#else
extern ALCdevice*  AL_OPaudioDevice;
extern ALCcontext* AL_OPaudioContext;
#endif
#endif

extern OPmutex OPAUD_CURR_MUTEX;
//-----------------------------------------------------------------------------


#ifdef OPIFEX_OPTION_AUDIO

//-----------------------------------------------------------------------------
//   ____                ______                _   _                 
//  / __ \              |  ____|              | | (_)                
// | |  | | __ _  __ _  | |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
// | |  | |/ _` |/ _` | |  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
// | |__| | (_| | (_| | | |  | |_| | | | | (__| |_| | (_) | | | \__ \
//  \____/ \__, |\__, | |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//          __/ | __/ |                                              
//         |___/ |___/                                               
typedef int (*LPOVCLEAR)(OggVorbis_File *vf);
typedef long (*LPOVREAD)(OggVorbis_File *vf,char *buffer,int length,int bigendianp,int word,int sgned,int *bitstream);
typedef ogg_int64_t (*LPOVPCMTOTAL)(OggVorbis_File *vf,int i);
typedef vorbis_info * (*LPOVINFO)(OggVorbis_File *vf,int link);
typedef vorbis_comment * (*LPOVCOMMENT)(OggVorbis_File *vf,int link);
typedef int (*LPOVOPENCALLBACKS)(void *datasource, OggVorbis_File *vf,char *initial, long ibytes, ov_callbacks callbacks);

extern LPOVCLEAR           fn_ov_clear;
extern LPOVREAD            fn_ov_read;
extern LPOVPCMTOTAL        fn_ov_pcm_total;
extern LPOVINFO            fn_ov_info;
extern LPOVCOMMENT         fn_ov_comment;
extern LPOVOPENCALLBACKS   fn_ov_open_callbacks;
//-----------------------------------------------------------------------------

#endif

//-----------------------------------------------------------------------------
//   ____  _____                _ _         ______                _   _                 
//  / __ \|  __ \              | (_)       |  ____|              | | (_)                
// | |  | | |__) |_ _ _   _  __| |_  ___   | |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
// | |  | |  ___/ _` | | | |/ _` | |/ _ \  |  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
// | |__| | |  | (_| | |_| | (_| | | (_) | | |  | |_| | | | | (__| |_| | (_) | | | \__ \
//  \____/|_|   \__,_|\__,_|\__,_|_|\___/  |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
OPint OPaudInit();

OPvec3 OPaudEarPosition(OPvec3* pos);
OPvec3 OPaudEarVelocity(OPvec3* pos);
OPvec3 OPaudEarForwardUp(OPfloat* pos);