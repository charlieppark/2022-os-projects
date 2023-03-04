#ifndef __LIST_H__
#define __LIST_H__

#include <stdio.h>
#include <stdlib.h>
#include "base.h"

typedef char* Data;

typedef struct _node
{
	Data data;
	struct _node* next;
} Node;

typedef struct _list
{
	Node* head;
    Node* tail;
    int size;
} List;

List* l_init();

void l_insert(List* const list, const Data const data);

void l_remove(List* const list, const int index);

Data l_get(List* const list, const int index);

#endif // !__LIST_H__