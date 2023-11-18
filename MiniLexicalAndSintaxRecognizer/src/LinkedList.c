#include "LinkedList.h"



Node* CreateNode(LL_TYPE data)
{
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

Node* Find(Node* head, char target[20])
{
    Node* current = head;
    while (current != NULL)
    {
        if (strcmp(current->data.ID, target) == 0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void Add(Node** head, LL_TYPE data)
{
    Node* newNode = CreateNode(data);
    if (*head == NULL)
    {
        *head = newNode; // Se a lista estiver vazia, o novo nó se torna a cabeça
    }
    else
    {
        Node* current = *head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
    }
}

void Remove(Node** head, char target[20])
{
    Node* current = *head;
    Node* prev = NULL;

    while (current != NULL && (strcmp(current->data.ID, target) != 0))
    {
        prev = current;
        current = current->next;
    }

    if (current != NULL)
    {
        if (prev == NULL)
        {
            *head = current->next;
        }
        else
        {
            prev->next = current->next;
        }
        free(current);
    }
}

void PrintList(Node** head)
{
    Node* current = *head;
    while (current != NULL)
    {
        printf("ID: %s | Endereco: %d \n", current->data.ID, current->data.Address);
        current = current->next;
    }
    printf("\n");
}

void FreeList(Node** head)
{
    Node* current = *head;
    Node* next;

    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
}
