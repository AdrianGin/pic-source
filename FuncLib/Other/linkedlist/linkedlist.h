

#ifdef __cplusplus    /* Insert start of extern C construct */
extern "C" {
#endif

#ifndef _LINKEDLIST_H
#define _LINKEDLIST_H

#include <stdint.h>


typedef struct LIST_NODE_t
{
	struct LIST_NODE_t* next;
	struct LIST_NODE_t* prev;
   void* data;
} LIST_NODE_t;

typedef struct
{
   LIST_NODE_t* first;
   LIST_NODE_t* last;
} LINKED_LIST_t;


LIST_NODE_t* LL_NewNode(void* data);
void* LL_PopData(LINKED_LIST_t* linkedList);
void LL_AppendData(LINKED_LIST_t* linkedList, void* data);
void LL_InsertAfter(LINKED_LIST_t* linkedList, LIST_NODE_t* node, LIST_NODE_t* newNode);
void LL_InsertBefore(LINKED_LIST_t* linkedList, LIST_NODE_t* node, LIST_NODE_t* newNode);
void LL_InsertBeginning(LINKED_LIST_t* linkedList, LIST_NODE_t* newNode);
void LL_InsertEnd(LINKED_LIST_t* linkedList, LIST_NODE_t* newNode);
void LL_Remove(LINKED_LIST_t* linkedList, LIST_NODE_t* node);
uint16_t LL_Count(LINKED_LIST_t* linkedList);
LIST_NODE_t* LL_ReturnNodeFromIndex(LINKED_LIST_t* linkedList, uint16_t item);
void* LL_ReturnNodeDataFromIndex(LINKED_LIST_t* linkedList, uint16_t item);



#define LL_Malloc(size)		pvPortMalloc(size)
#define LL_Free(handle)		vPortFree(handle)


#endif

#ifdef __cplusplus  /* Insert end of extern C construct. */
}                   /* The C header file can now be */
#endif              /* included in either C or C++ code. */
