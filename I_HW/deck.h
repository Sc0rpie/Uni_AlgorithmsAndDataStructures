#ifndef DECK_H
#define DECK_H

typedef struct node {
    int data;
    struct node *next;
} Node;

typedef struct deck {
    Node *head;
    Node *tail;
    int id;
} Deck;

Deck *createDeck(int id);
int checkEmpty(Deck *d);
int checkFull(Deck *d);
void print(Node *n);
void pushFront(Deck *d, int n_data);
void pushBack(Deck *d, int n_data);
int popFront(Deck *d);

#endif // DECK_H