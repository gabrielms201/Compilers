#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <memory.h>
#include <stdlib.h>
#include <stdio.h>


typedef int LL_TYPE;
typedef struct 
{
    LL_TYPE data;
    struct Node* next;
} Node;

Node* CreateNode(LL_TYPE data);
Node* Find(Node* head, LL_TYPE  target);
void Add(Node** head, LL_TYPE data);
void Remove(Node** head, LL_TYPE target);
void PrintList(Node** head);
void FreeList(Node** head);

#endif // !LINKED_LIST_H
