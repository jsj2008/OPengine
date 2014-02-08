#pragma once
#ifndef OPEngine_Core_MathHelpers
#define OPEngine_Core_MathHelpers

#include "Types.h"

#if defined(OPIFEX_UNIX)	// linux math libs
#include <math.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h> // link with -lrt
#include <time.h>
#elif defined(OPIFEX_WINDOWS) 	// windows math libs
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#endif

#define OPpi 3.14159265359f
#define OPe  2.71828182846

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif
/**
* OPceil - math helper
*	Calculates the ceiling of a floating point number. For example
*	if 0.5 is passed as f, then 1 will be returned.
* @param f Floating point number.
* @return integer ceiling
*/
__inline OPint OPceil(OPfloat f){
#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID)  || defined(OPIFEX_OSX32) || defined(OPIFEX_OSX64)// linux implementation
	return (OPint)ceil(f);
#elif defined(OPIFEX_WINDOWS) 	// windows implementation
	return (OPint)ceil(f);
#endif
}

//-----------------------------------------------------------------------------
/**
* OPfloor - math helper
*	Calculates the floor of a floating point number. For example
*	if 0.5 is passed as f, then 0 will be returned.
* @param f Floating point number.
* @return integer floor
*/
__inline OPint OPfloor(OPfloat f){
#if defined(OPIFEX_UNIX) // linux implementation
	return (OPint)floor(f);
#elif defined(OPIFEX_WINDOWS) 	// windows implementation
	return (OPint)floor(f);
#endif
}

//-----------------------------------------------------------------------------
/**
* OPabs - math helper
*	Calculates the absolute value of a floating point number.
* @param f Floating point number.
* @return absolute value
*/
__inline OPfloat OPabs(OPfloat f){
	return f > 0 ? f : -f;
}

//-----------------------------------------------------------------------------
/**
* OPabsf - math helper
*	Explicitly calculates the absolute value of a floating point number.
* @param f Floating point number.
* @return absolute value
*/
__inline OPfloat OPabsf(OPfloat f){
#if defined(OPIFEX_UNIX) // linux implementation
	return OPabs(f);
#elif defined(OPIFEX_WINDOWS) 	// windows implementation
	return OPabs(f);
#endif
}

//-----------------------------------------------------------------------------
/**
* OPabsi - math helper
*	Calculates the absolute value of an integer number.
* @param i integer number.
* @return absolute value
*/
__inline OPint	OPabsi(OPint i){
#if defined(OPIFEX_UNIX) // linux implementation
	return (OPint)OPabs(i);
#elif defined(OPIFEX_WINDOWS) 	// windows implementation
	return (OPint)OPabs(i);
#endif
}

//-----------------------------------------------------------------------------
/**
* OPsin - math helper
*	Calculates the sine of a floating point number.
* @param f Floating point number.
* @return sin(f)
*/
__inline OPfloat OPsin(OPfloat f){
#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_OSX32) || defined(OPIFEX_OSX64)	// linux implementation
	return sin(f);
#elif defined(OPIFEX_WINDOWS) 	// windows implementation
	return sin(f);
#elif defined(OPIFEX_ANDROID)	// android implementation
	return sin(f);
#endif
}

//-----------------------------------------------------------------------------
/**
* OPcos - math helper
*	Calculates the cosine of a floating point number.
* @param f Floating point number.
* @return cos(f)
*/
__inline OPfloat OPcos(OPfloat f){
#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_OSX32) || defined(OPIFEX_OSX64)	// linux implementation
	return cos(f);
#elif defined(OPIFEX_WINDOWS) 	// windows implementation
	return cos(f);
#elif defined(OPIFEX_ANDROID)	// android implementation
	return cos(f);
#endif
}

//-----------------------------------------------------------------------------
/**
* OPtan - math helper
*	Calculates the tangent of a floating point number.
* @param f Floating point number.
* @return tan(f)
*/
__inline OPfloat OPtan(OPfloat f){
#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_OSX32) || defined(OPIFEX_OSX64)	// linux implementation
	return tan(f);
#elif defined(OPIFEX_WINDOWS) 	// windows implementation
	return tan(f);
#elif defined(OPIFEX_ANDROID)	// android implementation
	return tan(f);
#endif
}

//-----------------------------------------------------------------------------
/**
* OPasin - math helper
*	Calculates the arc sine of a floating point number.
* @param f Floating point number.
* @return asin(f)
*/
__inline OPfloat OPasin(OPfloat f){
#if defined(OPIFEX_UNIX) // linux implementation
	return asin(f);
#elif defined(OPIFEX_WINDOWS) 	// windows implementation
	return asin(f);
#endif
}

//-----------------------------------------------------------------------------
/**
* OPacos - math helper
*	Calculates the arc cosine of a floating point number.
* @param f Floating point number.
* @return acos(f)
*/
__inline OPfloat OPacos(OPfloat f){
#if defined(OPIFEX_UNIX) // linux implementation
	return acos(f);
#elif defined(OPIFEX_WINDOWS) 	// windows implementation
	return acos(f);
#endif
}

//-----------------------------------------------------------------------------
/**
* OPatan - math helper
*	Calculates the arc tangent of a floating point number.
* @param f Floating point number.
* @return atan(f)
*/
__inline OPfloat OPatan(OPfloat f){
#if defined(OPIFEX_UNIX) // linux implementation
	return atan(f);
#elif defined(OPIFEX_WINDOWS) 	// windows implementation
	return atan(f);
#endif
}

//-----------------------------------------------------------------------------
/**
* OPpow - math helper
*	Raises b to a power.
* @param b base value
* @param exp power
* @return b^exp
*/
__inline OPfloat OPpow(OPfloat b, OPfloat exp){
#if defined(OPIFEX_UNIX) // linux implementation
	return pow(b, exp);
#elif defined(OPIFEX_WINDOWS) 	// windows implementation
	return pow(b, exp);
#endif
}

//-----------------------------------------------------------------------------
/**
* OPsqrt - math helper
*	Calculates the square root of a floating point number.
* @param f Floating point number.
* @return square of f
*/
__inline OPfloat OPsqrt(OPfloat f){
#if defined(OPIFEX_UNIX) // linux implementation
	return sqrt(f);
#elif defined(OPIFEX_WINDOWS) 	// windows implementation
	return sqrt(f);
#endif
}

//-----------------------------------------------------------------------------
/**
* OPlog10 - math helper
*	Calculates the base 10 log of a floating point number.
* @param f Floating point number.
* @return log10(f)
*/
__inline OPfloat OPlog10(OPfloat f){
#if defined(OPIFEX_UNIX) // linux implementation
	return log10(f);
#elif defined(OPIFEX_WINDOWS) 	// windows implementation
	return log10(f);
#endif
}

//-----------------------------------------------------------------------------
/**
* OPlog2 - math helper
*	Calculates the base 2 log of a floating point number.
* @param f Floating point number.
* @return log2(f)
*/
__inline OPfloat OPlog2(OPfloat f){
	return OPlog10(f) / OPlog10(2);
}

//-----------------------------------------------------------------------------
/**
* OPln - math helper
*	Calculates the natural log of a floating point number.
* @param f Floating point number.
* @return ln(f)
*/
__inline OPfloat OPln(OPfloat f){
#if defined(OPIFEX_UNIX) // linux implementation
	return log(f);
#elif defined(OPIFEX_WINDOWS) 	// windows implementation
	return log(f);
#endif
}

//-----------------------------------------------------------------------------
/**
* OPround - math helper
*	Returns a rounded number to the nearest whole number.
* @param f Number to round to the nearest whole number
* @return Floating point value
*/
__inline OPfloat OPround(OPfloat f){
#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID) // linux implementation
	return round(f);
#elif defined(OPIFEX_WINDOWS) 	// windows implementation
	return OPfloor(f + 0.5f);
#endif
}

//-----------------------------------------------------------------------------
/**
* OPnear - math helper
*	Determines if a value is within a threashold of another
* @param value Value to check
* @param target Target which value is checked against
* @return True if near is within theshhold of target, otherwise false.
*/
__inline OPfloat OPnear(OPfloat value, OPfloat target, OPfloat threshhold){
	return (OPabs(value - target) < threshhold ? 1.0f : 0.0f);
}


//-----------------------------------------------------------------------------
/**
* OPrandom - math helper
*	Returns a randomized floating point number.
* @return Floating point value with the range(0, 1)
*/
OPfloat OPrandom();

//-----------------------------------------------------------------------------
/**
* OPrandom - math helper
*	Returns a randomized floating point number.
* @param min Smallest possible number which can be generated
* @param max Largest possible number which can be generated
* @return Floating point value with the range(min, max)
*/
OPfloat OPrandRange(OPfloat min, OPfloat max);

#ifdef __cplusplus
}
#endif
#endif
