#include "./../include/Timer.h"
#include <stdio.h>

//----------------------------------------------------------------------------
OPtimer* OPcreateTimer(){
	OPtimer* timer = (OPtimer*)OPalloc((OPuint)sizeof(OPtimer));
	
#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
	gettimeofday(&(timer->_lastTime), NULL);
	timer->TotalGametime = 0;
	timer->TimeLastTick = 0;
	timer->Elapsed = 0;
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
// Windows specific values for time
#elif defined(OPIFEX_ANDROID)
// android specific values for time
#endif

	return timer;
}
//----------------------------------------------------------------------------
void OPdestroyTimer(OPtimer* timer){
	if(timer) OPfree(timer);
}
//----------------------------------------------------------------------------
void OPtimerTick(OPtimer* timer){
#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
	struct timeval time;
	ui64 elapsed;

	gettimeofday(&time, NULL);
	elapsed = (time.tv_sec - timer->_lastTime.tv_sec) * 1000000 + 
		  (time.tv_usec - timer->_lastTime.tv_usec);

	timer->TotalGametime += elapsed;
	timer->TimeLastTick = (time.tv_sec * 1000000 + time.tv_usec);
	timer->Elapsed = elapsed;
	
	timer->_lastTime = time;
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
// Windows specific values for time
#elif defined(OPIFEX_ANDROID)
// android specific values for time
#endif	
}
OPfloat  OPtimerDelta(OPtimer* timer){
	return (OPfloat)(timer->Elapsed / 1000000.0);
}
ui64 OPtimerTotal(OPtimer* timer){
	return timer->TotalGametime / 1000000.0;
}
