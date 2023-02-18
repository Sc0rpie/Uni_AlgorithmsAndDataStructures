#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "deck.h"

int checkID(int idToCheck, int deckCounter, Deck **deck)
{
    for (int i = 0; i < deckCounter; i++)
    {
        if (deck[i]->id == idToCheck)
            return 0;
    }
    return -1;
}

Deck *createDeck(int id)
{   
    Deck *d = (Deck *)malloc(sizeof(Deck));
    if (d == NULL)
    {
        printf("Memory allocation error\n");
        return NULL;
    }
    d->id = id;
    d->head = NULL;
    d->tail = NULL;
    return d;
}

int checkEmpty(Deck *d)
{
    if (d->head == NULL || d->tail == NULL)
        return 0;
    return -1;
}

int checkFull(Deck *d)
{
    if (d->head != NULL)
        return 0;
    return -1;
}

void print(Node *n) /* Sąrašo rašymas į konsolę */
{
    while (n != NULL)
    {
        printf(" %d ", n->data);
        n = n->next;
    }
    printf("\n");
}

void pushFront(Deck *d, int n_data) /* n_data - new data */
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->data = n_data;
    new_node->next = d->head;
    new_node->prev = NULL;
    if (d->head == NULL)
        d->tail = new_node;
    else
        d->head->prev = new_node;
    d->head = new_node;
}

void pushBack(Deck *d, int n_data) /* n_data - new data */
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->data = n_data;
    new_node->next = NULL;
    new_node->prev = d->tail;
    if (d->head == NULL)
    {
        d->head = new_node;
    }
    else
        d->tail->next = new_node;
    d->tail = new_node;
}

void popFront(Deck *d, int *value)
{
    if(checkEmpty(d) == 0)
    {
        printf("Unable to pop. Deck is empty!\n");
        return;
    }
    Node *temp = d->head;
    *value = temp->data;
    d->head = d->head->next;
    if (d->head == NULL)
        d->tail = NULL;
    else
        d->head->prev = NULL;
    free(temp);
    
    printf("Value popped: %d\n", *value);
}

void popBack(Deck *d, int *value)
{
    if(checkEmpty(d) == 0)
    {
        printf("Unable to pop. Deck is empty!\n");
        return;
    }
    Node *temp = d->tail;
    *value = temp->data;
    d->tail = d->tail->prev;
    if (d->tail == NULL)
        d->head = NULL;
    else
        d->tail->next = NULL;
    free(temp);
    printf("Value popped: %d\n", *value);
}