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
        // printf("Atminties paskirstymo klaida\n");
        printf("Klaida #1 - Atminties paskirstymo klaida\n");
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
    if (n == NULL)
    {
        // printf("Dekas yra tuscias\n"); //Deck is empty!
        printf("Klaida #4 - Dekas yra tuscias\n");
        return;
    }
    while (n != NULL)
    {
        printf("%d ", n->data);
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
        // printf("Neimanoma isimti elementa. Dekas yra tuscias!\n"); //Unable to pop. Deck is empty!
        printf("Klaida #5 - Neimanoma isimti elementa. Dekas yra tuscias!\n");
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
    
    printf("Isimta reiksme: %d\n", *value); //Value popped: 
}

void popBack(Deck *d, int *value)
{
    if(checkEmpty(d) == 0)
    {
        // printf("Neimanoma isimti elementa. Dekas yra tuscias!\n"); //Unable to pop. Deck is empty!
        printf("Klaida #5 - Neimanoma isimti elementa. Dekas yra tuscias!\n");
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
    printf("Isimta reiksme: %d\n", *value); //Value popped: 
}

void destroyDeck(Deck *d)
{
    Node *temp = d->head;
    while (temp != NULL)
    {
        d->head = d->head->next;
        free(temp);
        temp = d->head;
    }
    free(d);
}

//remove deck element from array and reallocate memory
Deck **removeDeckFromArr(Deck **deck, int *deckCounter, int id)
{
    int i = 0;
    while (i < *deckCounter)
    {
        if (deck[i]->id == id)
        {
            destroyDeck(deck[i]);
            for (int j = i; j < *deckCounter - 1; j++)
            {
                deck[j] = deck[j + 1];
            }
            *deckCounter -= 1;
            deck = (Deck **)realloc(deck, *deckCounter * sizeof(Deck *));
            return deck;
        }
        i++;
    }
    return deck;
}

void peekFront(Deck *d, int *peek)
{
    if(checkEmpty(d) == 0)
    {
        printf("Unable to peak. Deck is empty!\n");
        return;
    }
    *peek = d->head->data;
    printf("Peek: %d\n", *peek);
}

void peekRear(Deck *d, int *peek)
{
    if(checkEmpty(d) == 0)
    {
        printf("Unable to peak. Deck is empty!\n");
        return;
    }
    *peek = d->tail->data;
    printf("Peek: %d\n", *peek);
}

int elementCount(Deck *d)
{
    int count = 0;
    Node *temp = d->head;
    while (temp != NULL)
    {
        count++;
        temp = temp->next;
    }
    return count;
}