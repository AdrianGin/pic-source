


#include <stdint.h>

typedef struct
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

