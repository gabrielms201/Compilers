#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include "MiniLexicalAndSintaxRecognizer.h"


typedef TokenInfo LL_TYPE;
typedef struct 
{
    LL_TYPE data;
    struct Node* next;
} Node;

Node* CreateNode(LL_TYPE data);
Node* Find(Node* head, char target[20]);
void Add(Node** head, LL_TYPE data);
void Remove(Node** head, char target[20]);
void PrintList(Node** head);
void FreeList(Node** head);

#endif // !LINKED_LIST_H
