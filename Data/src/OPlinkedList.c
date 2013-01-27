#include "./../include/OPlinkedList.h"

//-----------------------------------------------------------------------------
OPlinkedList* OPllCreate(){
	OPlinkedList* list = (OPlinkedList*)OPalloc(sizeof(OPlinkedList));
	list->_size = 0;
	list->First = list->Last = NULL;
	return list;
}
//-----------------------------------------------------------------------------
OPint OPllDestroy(OPlinkedList* list){
	OPllNode* node = list->First;
	do{
		OPllNode* temp = node;
		node = node->Next;

		OPllRemove(list, temp);
	}while(node);
	return 1;
}
//-----------------------------------------------------------------------------
OPllNode* OPllInsertFirst(OPlinkedList* list, ui8* data){
	OPllNode* node = (OPllNode*)OPalloc(sizeof(OPllNode));
	node->Data = data;
	node->Next = node->Prev = NULL;

	if(list->First == NULL){
		list->Last = node;
	}
	else{
		node->Next = list->First;
		list->First->Prev = node;
	}

	list->First = node;

	return node;
}
//-----------------------------------------------------------------------------
OPllNode* OPllInsertLast(OPlinkedList* list, ui8* data){
	OPllNode* node = (OPllNode*)OPalloc(sizeof(OPllNode));
	node->Data = data;
	node->Prev = node->Next = NULL;

	if(list->Last == NULL){
		list->First = node;
	}
	else{
		node->Prev = list->Last;
		list->Last->Next = node;
	}

	list->Last = node;

	return node;
}
//-----------------------------------------------------------------------------
ui8* OPllRemove(OPlinkedList* list, OPllNode* toRemove){

	OPllNode *next = toRemove->Next, *prev = toRemove->Prev;
	ui8* data = toRemove->Data;

	if(prev) prev->Next = next;
	if(next) next->Prev = prev;

	OPfree(toRemove);
	list->_size--;

	return data;
}
//-----------------------------------------------------------------------------
OPint OPllGetSize(OPlinkedList* list){ return list->_size; }
//-----------------------------------------------------------------------------
//- C++ Definitions -----------------------------------------------------------
#ifdef __cplusplus // compile the C++ class
using namespace OPEngine::Data;

#endif
