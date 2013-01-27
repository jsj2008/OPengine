// TODO: Have option to use a FILE* to read from disk rather than memory

#pragma once
#include "./../../Core/include/Types.h"
#include "./../../Core/include/Target.h"

#ifndef OPEngine_Data_LinkedList
#define OPEngine_Data_LinkedList

#if defined(OPIFEX_ANDROID) && defined(__cplusplus)
#include "./../../Core/include/DynamicMemory.h"
#else
#include "./../../Core/include/Core.h"
#endif

typedef struct{
	struct OPllNode *Prev;
	struct OPllNode *Next;
	ui8* Data;
} OPllNode;

typedef struct{
	OPint _size;
	OPllNode* First;
	OPllNode* Last;
} OPlinkedList;

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif
	OPlinkedList* OPllCreate();
	OPint OPllDestroy(OPlinkedList* list);
	OPllNode* OPllInsertFirst(OPlinkedList* list, ui8* data);
	OPllNode* OPllInsertLast(OPlinkedList* list, ui8* data);
	ui8* OPllRemove(OPlinkedList* list, OPllNode* toRemove);
	OPint OPllGetSize(OPlinkedList* list);
#ifdef __cplusplus
};
//-----------------------------------------------------------------------------
//- C++ Definitions -----------------------------------------------------------
namespace OPEngine{
namespace Data{

}
}
#endif
#endif
