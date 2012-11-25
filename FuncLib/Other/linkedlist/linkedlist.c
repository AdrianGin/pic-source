


#include <stdint.h>
#include <stdlib.h>
#include "linkedlist\linkedlist.h"



LIST_NODE_t* LL_NewNode(void* data)
{
    LIST_NODE_t* newNode;

    newNode = (LIST_NODE_t*)LL_Malloc(sizeof(LIST_NODE_t));
//    while( newNode == NULL )
//    {
//        newNode = (LIST_NODE_t*)malloc(sizeof(LIST_NODE_t));
//    }

    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->data = data;
    return (LIST_NODE_t*)newNode;
}

void* LL_PopData(LINKED_LIST_t* linkedList)
{
    LIST_NODE_t* retNode;
    void* data = NULL;
    retNode = linkedList->first;
    if( retNode )
    {
        data = retNode->data;
    }

    LL_Remove(linkedList, retNode);
    return data;
}

void LL_AppendData(LINKED_LIST_t* linkedList, void* data)
{
    LIST_NODE_t* newNode = LL_NewNode(data);
    LL_InsertEnd(linkedList, newNode);
}


void LL_InsertAfter(LINKED_LIST_t* linkedList, LIST_NODE_t* node, LIST_NODE_t* newNode)
{
     newNode->prev = (struct LIST_NODE_t*)node;
     newNode->next = node->next;
     if( node->next == NULL)
     {
        linkedList->last = newNode;
     }
     else
     {
         LIST_NODE_t* tmp;
         tmp = (LIST_NODE_t*)node->next;
         tmp->prev = (struct LIST_NODE_t*)newNode;
     }
     node->next = (struct LIST_NODE_t*)newNode;
}

void LL_InsertBefore(LINKED_LIST_t* linkedList, LIST_NODE_t* node, LIST_NODE_t* newNode)
{
   newNode->prev = node->prev;
   newNode->next = (struct LIST_NODE_t*)node;
   if( node->prev == NULL )
   {
      linkedList->first = newNode;
   }
   else
   {
       LIST_NODE_t* tmp;
       tmp = (LIST_NODE_t*)node->prev;
       tmp->next = (struct LIST_NODE_t*)newNode;
       node->prev = (struct LIST_NODE_t*)newNode;
   }
}

void LL_InsertBeginning(LINKED_LIST_t* linkedList, LIST_NODE_t* newNode)
{
    if(linkedList->first == NULL)
    {
       linkedList->first = newNode;
       linkedList->last  = newNode;
       newNode->prev = NULL;
       newNode->next = NULL;
    }
    else
    {
        LL_InsertBefore(linkedList, linkedList->first, newNode);
    }
}

void LL_InsertEnd(LINKED_LIST_t* linkedList, LIST_NODE_t* newNode)
{
    if( linkedList->last == NULL )
    {
       LL_InsertBeginning(linkedList, newNode);
    }
    else
    {
       LL_InsertAfter(linkedList, linkedList->last, newNode);
    }
}

void LL_Remove(LINKED_LIST_t* linkedList, LIST_NODE_t* node)
{

   if( node == NULL)
   {
      return;
   }
   //If we are removing the head
   if( node->prev == NULL )
   {
      linkedList->first = (LIST_NODE_t*)node->next;
   }
   else
   {
      LIST_NODE_t* tmp;
      tmp = (LIST_NODE_t*)node->prev;
      tmp->next = (struct LIST_NODE_t*)node->next;
   }
   //If we are removing the tail
   if( node->next == NULL)
   {
      linkedList->last = (LIST_NODE_t*)node->prev;
   }
   else
   {
      LIST_NODE_t* tmp;
      tmp = (LIST_NODE_t*)node->next;
      tmp->prev = (struct LIST_NODE_t*)node->prev;
   }
   if( node != NULL)
   {
	   LL_Free(node);
   }
   node = NULL;
}


uint16_t LL_Count(LINKED_LIST_t* linkedList)
{
	LIST_NODE_t* tmp;
	uint16_t count = 0;

	tmp = linkedList->first;
	while(tmp != NULL)
	{
		count++;
		tmp = tmp->next;
	}

	return count;
}


LIST_NODE_t* LL_ReturnNodeFromIndex(LINKED_LIST_t* linkedList, uint16_t item)
{
	LIST_NODE_t* tmp;
	uint16_t count = LL_Count(linkedList);

	if(item >= count)
	{
		return NULL;
	}


	tmp = linkedList->first;
	while( (tmp != NULL) && (item) )
	{
		item--;
		tmp = tmp->next;
	}

	return tmp;
}



void* LL_ReturnNodeDataFromIndex(LINKED_LIST_t* linkedList, uint16_t item)
{
	LIST_NODE_t* tmp;

	tmp = LL_ReturnNodeFromIndex(linkedList, item);

	if( tmp != NULL )
	{
		return tmp->data;
	}
	return NULL;
}



