#include "./Core/include/OPmemory.h"

#ifndef OPIFEX_OPTION_RELEASE
OPint OPallocations = 0;
OPint OPdeallocations = 0;
#endif

OPchar OPSCRATCHBUFFER[OPSCRATCHBUFFER_SIZE];

void* OPallocatorBasicAlloc(void* alloc, OPuint bytes) {
	#ifndef OPIFEX_OPTION_RELEASE
		OPallocations++;
	#endif

		// allocate memory (for each platform)
	#if defined(OPIFEX_UNIX)
		// android specific for malloc
		return malloc(bytes);
	#elif defined(OPIFEX_WINDOWS)
		// Windows specific for malloc
		return malloc(bytes);
	#endif
}

void OPallocatorBasicFree(void* alloc, void* ptr) {
	#ifndef OPIFEX_OPTION_RELEASE
		OPdeallocations++;
	#endif
	// free allocated memory (for each platform)
	#if defined(OPIFEX_UNIX)
	// android specific for malloc
	if(ptr) free(ptr);
	#elif defined(OPIFEX_WINDOWS)
	// Windows specific for malloc
	if(ptr) free(ptr);
	#endif
}

void OPallocatorBasicReset(void* alloc) { }

OPallocator OPDEFAULT_ALLOCATOR = {
	NULL,
	OPallocatorBasicAlloc,
	OPallocatorBasicFree,
	OPallocatorBasicReset
};

//-----------------------------------------------------------------------------
/* OPalloc(..) - Platform independent means to dynamically allocate memory.
*	Parameters:
*		uint bytes - Number of consecutive bytes to be allocated.
*	Returns:
*		void* - Address of allocated memory.
*/
void* OPalloc(OPuint bytes){
	return OPDEFAULT_ALLOCATOR.alloc(&OPDEFAULT_ALLOCATOR, bytes);
}

//-----------------------------------------------------------------------------
/* OPallocZero(..) - Platform independent means to dynamically allocate memory and zero it out.
*	Parameters:
*		uint bytes - Number of consecutive bytes to be allocated.
*	Returns:
*		void* - Address of allocated memory.
*/
void* OPallocZero(OPuint bytes){
	void* result;
	result = OPDEFAULT_ALLOCATOR.alloc(&OPDEFAULT_ALLOCATOR, bytes);
 	OPbzero(result, bytes);
 	return result;
}
//-----------------------------------------------------------------------------
/* OPrealloc(..) - Platform independent means to dynamically reallocate memory.
 *	Parameters:
 *		void* ptr - Existing segment of allocated memory
 *		uint  bytes - Number of consecutive bytes to be allocated.
 *	Returns:
 *		void* - Address of allocated memory.
 */
void* OPrealloc(void* ptr, OPuint bytes){
	// allocate memory (for each platform)
	#if defined(OPIFEX_UNIX)
	// android specific for malloc
	return realloc(ptr, bytes);
	#elif defined(OPIFEX_WINDOWS)
	// Windows specific for malloc
	return realloc(ptr, bytes);
	#endif
}
//-----------------------------------------------------------------------------
/* OPfree(..) - Platform independent means do deallocate dynamically
 *		allocated memory. If a null pointer is passed no action
 *		is taken.
 *	Parameters:
 *		void* ptr - Address to the memory segment to deallocate
 *	Returns:
 *		Nothing...
 */
void OPfree(void* ptr){
	OPDEFAULT_ALLOCATOR.free(&OPDEFAULT_ALLOCATOR, ptr);
}


void* OPsysAlloc(OPuint bytes) {
	return malloc(bytes);
}

void OPsysFree(void* ptr) {
	free(ptr);
}
