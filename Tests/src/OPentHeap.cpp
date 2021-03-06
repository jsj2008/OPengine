#include "Tests/include/helpers.h"
#include "Core/include/OPmath.h"
#include "Data/include/OPentHeap.h"

OP_TEST_INIT

typedef struct{
	OPint id;
} Dummy;
void* mem;
OPentHeap* heap;
OPint bytes;

//-----------------------------------------------------------------------------
//    _______        _     ______                      
//   |__   __|      | |   |  ____|                     
//      | | ___  ___| |_  | |__ _   _ _ __   ___ ___   
//      | |/ _ \/ __| __| |  __| | | | '_ \ / __/ __|  
//      | |  __/\__ \ |_  | |  | |_| | | | | (__\__ \_ 
//      |_|\___||___/\__| |_|   \__,_|_| |_|\___|___(_)                                               
OPint shouldCreateHeapWithCapacity(void* args){
	OPuint size = (OPuint)OPrandRange(1, 100);
	mem = OPalloc(bytes = OPentHeapBytes(sizeof(Dummy), size));

	OP_RTMSG("Creating a heap with capacity of %d", size);
	heap = OPentHeapCreate(mem, sizeof(Dummy), size);
	OP_RTMSG("OPint Size: %d", sizeof(OPint));
	OP_RTMSG("HEAP %x", heap);
	OP_RTMSG("Entities: %x", &heap->Entities);
	OP_RTMSG("InUse: %x", &heap->InUse);
	OP_RTMSG("EntSize: %x", &heap->EntSize);
	OP_RTMSG("MaxIndex: %x", &heap->MaxIndex);
	OP_RTMSG("Free: %x", heap->Free);


	OP_RTMSG("Free._indices %x", heap->Free._indices);
	OP_RTMSG("heap.EntSize %x", heap->EntSize);
	OP_RTMSG("Free._capacity %x", heap->Free._capacity);
	OP_RTMSG("Free._size %x", heap->Free._size);
	if(!heap){
		OP_RTMSG("Heap was null!");
		return -1;
	}


	if(heap->Free._indices == NULL){
		OP_RTMSG("Indicies not allocated! %x", heap->Free._indices);
		return -3;
	}

	if(*heap->Size != size){
		OP_RTMSG("Size not correct %d", *heap->Size);
		return -4;
	}
	OP_RTMSG("Size %d=%d", *heap->Size, size);
	OP_RTMSG("Indicies allocated!");

	if(((ui8*)heap->Size) < ((ui8*)heap) || ((ui8*)heap->Size) > ((ui8*)(heap + bytes))){
		OP_RTMSG("Size pointer outsize of expected bounds %x - %x", heap, heap + bytes);
		return -5;
	}

	OP_HEX_DUMP((void*)heap, bytes);

	return 0;
}
//-----------------------------------------------------------------------------
OPint pushAndPopShouldReturnSorted(void* args){
	OPint i = 0, last = 0;
	OPuint size = *heap->Size;	
	
	OP_RTMSG("Size %d bytes %d", size, bytes);

	// create a list of random numbers
	for(i = size; i--;){
		OPint index = -1;
		Dummy dummy = {0};
		OPentHeapActivate(heap, &index);
		(*((Dummy*)OPentHeapGet(heap, index))).id = (OPint)(OPrandom() * 1024);
	}

	// remove one and check for the next available
	for(i = 10; i--;){
		OPint activated = -1, released = (OPint)(OPrandom() * size); 
		OP_RTMSG("Killing index %d...", released);
		OPentHeapKill(heap, released);
		OP_RTMSG("Expecting %d as next activated", released);
	
		OPentHeapActivate(heap, &activated);
		OP_RTMSG("Activated %d expected %d", activated, released);
		
		if(activated != released) return -1;
	}	

	OP_RTMSG("Size %d", size);
	OP_RTMSG("Size: %x -> %x", heap->Size, &heap->Free._size);

	OP_HEX_DUMP((void*)heap, bytes);

	return 0;
}
//-----------------------------------------------------------------------------
OPint maxIndexExpected(void* args){
	OPint i = 0, max = 0;
	OPuint size = *heap->Size;

	OP_RTMSG("Off to size %x", ((ui8*)heap->Size - (ui8*)heap));
	OP_RTMSG("Size %d Cap %d bytes %d", size, heap->Capacity, bytes);
	OP_RTMSG("Size: %x -> %x", heap->Size, &heap->Free._size);

	// clean out existing entities
	for(i = heap->Capacity; i--;){
		OPentHeapIsLiving(heap, i)

		OPentHeapKill(heap, i);
		OP_RTMSG("Killing at index %d", i);
	}

	// create a list of random numbers
	for(i = heap->Capacity; i--;){
		OPint activated = -1;
		Dummy dummy = {0};

		OPentHeapActivate(heap, &activated);
		((Dummy*)OPentHeapGet(heap, activated))->id = (OPint)(OPrandom() * 1024);
		OP_RTMSG("Activated %d", activated);
	
		max = activated > max ? activated : max;
	}	

	while(heap->MaxIndex > 1){
		OP_RTMSG("Max index before: %d, expected after: %d", heap->MaxIndex, max);
		OP_RTMSG("MaxIndex %d, now killing...", max);
		OPentHeapKill(heap, max);

		if(heap->MaxIndex != max){
			OP_RTMSG("%d does not match expected %d!", heap->MaxIndex, max);
			OP_HEX_DUMP((void*)heap, bytes);
			return -2;
		}
		OP_RTMSG("Max index %d expected %d\n", heap->MaxIndex, max);
		max = heap->MaxIndex - 1;
	}

	OP_HEX_DUMP((void*)heap, bytes);

	return 0;
}
/*
//-----------------------------------------------------------------------------
//    _______        _     _____                             
//   |__   __|      | |   |  __ \                            
//      | | ___  ___| |_  | |__) |   _ _ __  _ __   ___ _ __ 
//      | |/ _ \/ __| __| |  _  / | | | '_ \| '_ \ / _ \ '__|
//      | |  __/\__ \ |_  | | \ \ |_| | | | | | | |  __/ |   
//      |_|\___||___/\__| |_|  \_\__,_|_| |_|_| |_|\___|_|   
*/
int main(void){
	OPint result = 0;

	// Run sequence of test function invocations here
	result |= OP_TEST(shouldCreateHeapWithCapacity, "Creation test", NULL);
	result |= OP_TEST(pushAndPopShouldReturnSorted, "Push / Pop test", NULL);
	result |= OP_TEST(maxIndexExpected, "Max index test", NULL);

	return (int)result; // value to be read by test script
}
