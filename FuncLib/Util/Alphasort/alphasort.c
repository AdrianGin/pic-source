
#include "linkedlist/linkedlist.h"
#include <ctype.h>
#include <stdlib.h>
#include "alphasort.h"
//
#define KEYS_SAME	(0)
#define KEY1_LARGER (1)
#define KEY2_LARGER	(2)
uint8_t DO_CHAR_TEST(char key, char key2)
{
	if( key != key2)
	{
		if( key > key2 )
		{
			return KEY1_LARGER;
		}
		else
		{
			return KEY2_LARGER;
		}
	}

	return KEYS_SAME;
}


//Give weightings to each.
#define CHAR_OFFSET (32)
uint8_t CONVERT_CHAR(char key)
{
	uint8_t ret;

	if( isspace(key) )
	{
		ret = key;
	}
	//0

	if( ispunct(key) )
	{
		ret = key;
	}
	//94

	if( isdigit(key) )
	{
		ret = key - '0' + ('~') + 1;
	}

	if( isupper(key))
	{
		ret = key - 'A' + ('~') + 1 + ('9' - '0') + 1;
	}

	if( islower(key) )
	{
		ret = toupper(key) - 'A' + ('~') + 1 + ('9' - '0') + 1;
	}

	return ret;
}

uint8_t DO_TEST(char* key, char* key2)
{
	uint8_t ret;

	if( key == NULL || key2 == NULL)
	{
		return 0;
	}

	while(*key && *key2)
	{
		uint8_t char1;
		uint8_t char2;

		char1 = CONVERT_CHAR(*key);
		char2 = CONVERT_CHAR(*key2);

		ret = DO_CHAR_TEST(char1, char2);
		if( ret )
		{
			return ret;
		}
		key++;
		key2++;
	}

	//If Key1 ends before Key2 and they are the same, return Key2 larger.
	return DO_CHAR_TEST(*key, *key2);

}



void alphasort_linkedList(LINKED_LIST_t* linkedList, uint8_t mode)
{

	int i, j;
	int count;

	char* key;
	char* key2;
	LIST_NODE_t* node;

	count = LL_Count(linkedList);

	for (j = 0; j < count; j++)    // Start with 1 (not 0)
	{
		for (i = 0; i <= (count); i++) // Smaller values move up
		{
			uint8_t result;
			uint8_t doMove = 0;
			key =  (char*)LL_ReturnNodeDataFromIndex(linkedList, i);
			key2 = (char*)LL_ReturnNodeDataFromIndex(linkedList, i+1);

			result = DO_TEST(key, key2);
			if( result )
			{
			    LIST_NODE_t* Key1Node = LL_ReturnNodeFromIndex(linkedList, i);
			    LIST_NODE_t* Key2Node = LL_ReturnNodeFromIndex(linkedList, i+1);

			    if( mode == SORT_DESCENDING)
			    {
					if(result == KEY2_LARGER)
					{
						doMove = 1;
					}
			    }
			    else
			    {
					if(result == KEY1_LARGER)
					{
						doMove = 1;
					}
			    }

			    if( doMove )
			    {
					LL_Remove(linkedList, Key2Node);
					Key2Node = LL_NewNode(key2);
					LL_InsertBefore(linkedList, Key1Node, Key2Node);
			    }
			}
		}
	}
}







