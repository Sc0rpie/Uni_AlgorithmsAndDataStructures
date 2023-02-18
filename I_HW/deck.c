#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "deck.h"

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
    if (d->head == NULL)
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
    if (d->head == NULL)
        d->tail = new_node;
    d->head = new_node;
}

void pushBack(Deck *d, int n_data) /* n_data - new data */
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->data = n_data;
    new_node->next = NULL;
    if (d->head == NULL)
    {
        d->head = new_node;
        d->tail = new_node;
        return;
    }
    d->tail->next = new_node;
    d->tail = new_node;
}

int popFront(Deck *d)
{
    // checkEmpty(d);
    Node *temp = d->head;
    int data = temp->data;
    d->head = d->head->next;
    free(temp);
    return data;
}