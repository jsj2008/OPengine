#ifndef OPEngine_Core_Types
#define OPEngine_Core_Types

#include "Target.h"
// define mathematical types

// integer types
typedef signed long    i64;
typedef unsigned long  ui64;  //long type
typedef signed int     i32;
typedef unsigned int   ui32; //int type
typedef signed short   i16;
typedef unsigned short ui16; //short type
typedef signed char    i8;
typedef unsigned char  ui8;  //byte type

// floating point types
typedef float  f32; //float type
typedef double d64; //double type

// Define cross platform base types
#if defined(OPIFEX_ANDROID) || defined(OPIFEX_WIN32) || defined(OPIFEX_LINUX32)
typedef i32 OPint;
typedef ui32 OPuint;
typedef f32 OPfloat;
#elif defined(OPIFEX_WIN64) || defined(OPIFEX_LINUX64)
typedef i64 OPint;
typedef ui64 OPint;
typedef f64 OPfloat;
#else
// default to building 32 bit binaries
typedef i32 OPint;
typedef ui32 OPuint;
typedef f32 OPfloat;
#endif

// string types
typedef struct{
	ui8* Data;
	OPuint Length;
} OPstring;

#endif