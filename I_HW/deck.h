#ifndef DECK_H
#define DECK_H

typedef struct node {
    int data;
    struct node *next;
    struct node *prev;
} Node;

typedef struct deck {
    Node *head;
    Node *tail;
    int id;
} Deck;

int checkID(int idToCheck, int deckCounter, Deck **deck);
Deck *createDeck(int id);
int checkEmpty(Deck *d);
int checkFull(Deck *d);
void print(Node *n);
void pushFront(Deck *d, int n_data);
void pushBack(Deck *d, int n_data);
void popFront(Deck *d, int *value);
void popBack(Deck *d, int *value);

#endif // DECK_H