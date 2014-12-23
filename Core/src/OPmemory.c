#include "./Core/include/OPmemory.h"

//-----------------------------------------------------------------------------
/* OPalloc(..) - Platform independent means to dynamically allocate memory.
*	Parameters:
*		uint bytes - Number of consecutive bytes to be allocated.
*	Returns:
*		void* - Address of allocated memory.
*/
void* OPalloc(OPuint bytes){
	// allocate memory (for each platform)
#if defined(OPIFEX_UNIX)
	// android specific for malloc
	return malloc(bytes);
#elif defined(OPIFEX_WINDOWS)
	// Windows specific for malloc
	return malloc(bytes);
#endif
}

//-----------------------------------------------------------------------------
/* OPallocZero(..) - Platform independent means to dynamically allocate memory and zero it out.
*	Parameters:
*		uint bytes - Number of consecutive bytes to be allocated.
*	Returns:
*		void* - Address of allocated memory.
*/
void* OPallocZero(OPuint bytes){
	// allocate memory (for each platform)
#if defined(OPIFEX_UNIX)
	// android specific for malloc
	void* result = malloc(bytes);
	OPbzero(result, bytes);
	return result;
#elif defined(OPIFEX_WINDOWS)
	// Windows specific for malloc
	void* result = malloc(bytes);
	OPbzero(result, bytes);
	return result;
#endif
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
	// free allocated memory (for each platform)
	#if defined(OPIFEX_UNIX)
	// android specific for malloc
	if(ptr) free(ptr);
	#elif defined(OPIFEX_WINDOWS)
	// Windows specific for malloc
	if(ptr) free(ptr);
	#endif
}