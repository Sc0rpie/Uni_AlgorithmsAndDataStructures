// ADT - Deck (stack that can be accessed from both ends)

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "deck.h"

void waitForInput()
{
    printf("Paspauskite ENTER mygtuka..."); //Press ENTER to continue...
    fflush(stdin);
    while( getchar() != '\n' );
}

int askForData()
{
    printf("Iveskite skaiciu: "); //Enter data to push: 
    int data;
    scanf("%d", &data);
    return data;
}

int askForDeck(int deckCounter, Deck **deck)
{
    if (deckCounter == 0 || deck == NULL)
        return -2;

    int idToCheck;
    printf("Prieinami deku ID: "); //Available deck ID's: 
    for (int i = 0; i < deckCounter; i++)
    {
        printf("%d ", deck[i]->id);
    }
    printf("\nIveskite deko ID: "); //Enter deck ID: 
    scanf("%d", &idToCheck);
    if (checkID(idToCheck, deckCounter, deck) == 0)
        return idToCheck;
    else
        return -1;
}

int getPosition(Deck **deck, int deckCounter, int id)
{
    for (int i = 0; i < deckCounter; i++)
    {
        if (deck[i]->id == id)
            return i;
    }
    return -1;
}

Deck **addToArray(Deck **ptr_array, Deck *ptr, int *elementCount)
{
    // printf("Element count in func: %d\n", *elementCount);
    if (ptr_array == NULL)
    {
        ptr_array = (Deck **) malloc(sizeof(Deck *));
        ptr_array[0] = ptr;
        *elementCount = 1;
    }
    else
    {
        *elementCount = *elementCount + 1;
        ptr_array = (Deck **) realloc(ptr_array, sizeof(Deck *) * (*elementCount));
        ptr_array[*elementCount-1] = ptr;
    }
    return ptr_array;
}

void menu()
{
    bool showMenu = true;
    int choice;
    int idCount = 0; // deck id counter
    int deckCounter = 0; // deck counter
    Deck **deck_arr = NULL; // array of decks
    Deck *dq;
    int idToCheck, dataToPush, position, pop, peek;
    while(showMenu)
    {
        fflush(stdin);
        system("clear");
        printf("[1] Sukurti deka\n"); //Create deck
        printf("[2] Patikrinti, ar dekas tuscias\n"); //Check if deck is empty
        printf("[3] Patikrinti, ar dekas pilnas\n"); //Check if deck is full
        printf("[4] Iterpti duomenis i deko pradzia\n"); //Push to front
        printf("[5] Iterpti duomenis i deko pabaiga\n"); //Push to back
        printf("[6] Isimti elementa is deko pradzios\n"); //Pop from front
        printf("[7] Isimti elementa is deko pabaigos\n"); //Pop from back
        printf("[8] Parasyti deko sudeti\n"); //Print deck
        printf("[9] Sunaikinti deka\n"); //Destroy deck
        printf("[0] Iseiti\n"); //Exit
        printf("Jusu pasirinkimas: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                dq = createDeck(idCount);
                idCount++;
                deck_arr = addToArray(deck_arr, dq, &deckCounter);
                // printf("DeckCntr: %d | idCount: %d\n", deckCounter, idCount);
                // deck = deckInit(&deckCounter, &idCount);
                // if(deckCounter == 0)
                // {
                //     deckCounter++;
                //     idCount++;
                //     deck = malloc(sizeof(Deck *) * deckCounter);
                //     if (deck == NULL)
                //     {
                //         printf("Error allocating memory");
                //         exit(1);
                //     }
                // }
                // else
                // {
                //     deckCounter++;
                //     idCount++;
                //     deck = realloc(deck, sizeof(Deck *) * deckCounter);
                //     if (deck == NULL)
                //     {
                //         printf("Error allocating memory");
                //         exit(1);
                //     }
                // }
                // printf("FUNC\n");
                // deck[idCount-1] = createDeck(deckCounter);
                system("clear");
                printf("Dekas sukurtas su ID: %d\n", idCount-1); //Deck created with id: 
                waitForInput();
                break;
            case 2:
                idToCheck = askForDeck(deckCounter, deck_arr);
                if (idToCheck == -2)
                    // printf("Nera prieinamu deku\n"); //No decks available
                    printf("Klaida #2\n");
                else if (idToCheck != -1)
                {
                    if (checkEmpty(deck_arr[idToCheck]) == 0)
                        printf("Dekas yra tuscias\n"); //Deck is empty.
                    else
                        printf("Dekas nera tuscias\n"); //Deck is not empty.
                }
                else
                {
                    // printf("Toks ID neegzistuoja.\n"); //Incorrect ID
                    printf("Klaida #3\n");
                }
                waitForInput();
                break;
            case 3:
                idToCheck = askForDeck(deckCounter, deck_arr);
                if (idToCheck == -2)
                    // printf("Nera prieinamu deku\n"); //No decks available
                    printf("Klaida #2\n");
                else if (idToCheck != -1)
                {
                    if (checkFull(deck_arr[idToCheck]) == 0)
                        printf("Dekas yra pilnas\n"); //Deck is full.
                    else
                        printf("Dekas nera pilnas\n"); //Deck is not full.
                }
                else
                {
                    // printf("Toks ID neegzistuoja.\n"); //Incorrect ID
                    printf("Klaida #3\n");
                }
                waitForInput();
                break;
            case 4:
                idToCheck = askForDeck(deckCounter, deck_arr);
                if (idToCheck == -2)
                    // printf("Nera prieinamu deku\n"); //No decks available
                    printf("Klaida #2\n");
                else if (idToCheck != -1)
                {
                    dataToPush = askForData();
                    position = getPosition(deck_arr, deckCounter, idToCheck);
                    pushFront(deck_arr[position], dataToPush);
                }
                else
                {
                    // printf("Toks ID neegzistuoja.\n"); //Incorrect ID
                    printf("Klaida #3\n");
                }
                waitForInput();
                break;
            case 5:
                idToCheck = askForDeck(deckCounter, deck_arr);
                if (idToCheck == -2)
                    // printf("Nera prieinamu deku\n"); //No decks available
                    printf("Klaida #2\n");
                else if (idToCheck != -1)
                {
                    dataToPush = askForData();
                    position = getPosition(deck_arr, deckCounter, idToCheck);
                    pushBack(deck_arr[position], dataToPush);
                }
                else
                {
                    // printf("Toks ID neegzistuoja.\n"); //Incorrect ID
                    printf("Klaida #3\n");
                }
                waitForInput();
                break;
            case 6:
                idToCheck = askForDeck(deckCounter, deck_arr);
                if (idToCheck == -2)
                    // printf("Nera prieinamu deku\n"); //No decks available
                    printf("Klaida #2\n");
                else if (idToCheck != -1)
                {
                    position = getPosition(deck_arr, deckCounter, idToCheck);
                    popFront(deck_arr[position], &pop);
                    // waitForInput();
                }
                else
                {
                    // printf("Toks ID neegzistuoja.\n"); //Incorrect ID
                    printf("Klaida #3\n");
                    // waitForInput();
                }
                waitForInput();
                break;
            case 7:
                idToCheck = askForDeck(deckCounter, deck_arr);
                if (idToCheck == -2)
                    // printf("Nera prieinamu deku\n"); //No decks available
                    printf("Klaida #2\n");
                else if (idToCheck != -1)
                {
                    position = getPosition(deck_arr, deckCounter, idToCheck);
                    popBack(deck_arr[position], &pop);
                    // waitForInput();
                }
                else
                {
                    // printf("Toks ID neegzistuoja.\n"); //Incorrect ID
                    printf("Klaida #3\n");
                    // waitForInput();
                }
                waitForInput();
                break;
            case 8:
                idToCheck = askForDeck(deckCounter, deck_arr);
                if (idToCheck == -2)
                    // printf("Nera prieinamu deku\n"); //No decks available
                    printf("Klaida #2\n");
                else if (idToCheck != -1)
                {
                    position = getPosition(deck_arr, deckCounter, idToCheck);
                    print(deck_arr[position]->head);
                }
                else
                {
                    // printf("Toks ID neegzistuoja.\n"); //Incorrect ID
                    printf("Klaida #3\n");
                }
                waitForInput();
                break;
            case 9:
                idToCheck = askForDeck(deckCounter, deck_arr);
                if (idToCheck == -2)
                    // printf("Nera prieinamu deku\n"); //No decks available
                    printf("Klaida #2\n");
                else if (idToCheck != -1)
                {
                    deck_arr = removeDeckFromArr(deck_arr, &deckCounter, idToCheck);
                    printf("Dekas su ID %d buvo istrintas\n", idToCheck); //Deck with id %d has been removed!
                }
                else
                {
                    // printf("Toks ID neegzistuoja.\n"); //Incorrect ID
                    printf("Klaida #3\n");
                }
                waitForInput();
                break;
            case 10:
                idToCheck = askForDeck(deckCounter, deck_arr);
                if (idToCheck == -2)
                    // printf("Nera prieinamu deku\n"); //No decks available
                    printf("Klaida #2\n");
                else if (idToCheck != -1)
                {
                    position = getPosition(deck_arr, deckCounter, idToCheck);
                    peekFront(deck_arr[position], &peek);
                    // printf("Value: %d", peekFront(deck_arr[position]));
                    // printf("Deck with id %d has %d elements\n", idToCheck, deck_arr[position]->size);
                }
                else
                {
                    // printf("Toks ID neegzistuoja.\n"); //Incorrect ID
                    printf("Klaida #3\n");
                }
                waitForInput();
                break;
            case 11:
                idToCheck = askForDeck(deckCounter, deck_arr);
                if (idToCheck == -2)
                    // printf("Nera prieinamu deku\n"); //No decks available
                    printf("Klaida #2\n");
                else if (idToCheck != -1)
                {
                    position = getPosition(deck_arr, deckCounter, idToCheck);
                    peekRear(deck_arr[position], &peek);
                    // printf("Value: %d", peekFront(deck_arr[position]));
                    // printf("Deck with id %d has %d elements\n", idToCheck, deck_arr[position]->size);
                }
                else
                {
                    // printf("Toks ID neegzistuoja.\n"); //Incorrect ID
                    printf("Klaida #3\n");
                }
                waitForInput();
                break;
            case 12:
                idToCheck = askForDeck(deckCounter, deck_arr);
                if (idToCheck == -2)
                    // printf("Nera prieinamu deku\n"); //No decks available
                    printf("Klaida #2\n");
                else if (idToCheck != -1)
                {
                    position = getPosition(deck_arr, deckCounter, idToCheck);
                    printf("Dekas su ID %d turi %d elementu\n", idToCheck, elementCount(deck_arr[position])); //Deck with id %d has %d elements
                }
                else
                {
                    // printf("Toks ID neegzistuoja.\n"); //Incorrect ID
                    printf("Klaida #3\n");
                }
                waitForInput();
                break;
            case 0:
                printf("Iseinama is programos...\n"); //Exiting program...
                // exit(0);
                showMenu = false;
                break;
            default:
                printf("Tokio pasirinkimo nera, pabandykite dar karta.\n"); //Invalid choice, try again
                waitForInput();
        }
    }
}

int main()
{   
    menu();
    // int elements = 0;
    // Deck *dq = createDeck(0);
    // Deck *dqq = createDeck(1);
    // pushFront(dq, 5);
    // pushFront(dq, 25);
    // pushFront(dqq, 12);
    // pushFront(dqq, 14);
    // Deck **ptr_arr = NULL;
    // ptr_arr = addToArray(ptr_arr, dq, &elements);
    // ptr_arr = addToArray(ptr_arr, dqq, &elements);
    // printf("Elements: %d\n", elements);
    // print(ptr_arr[0]->head);
    // print(ptr_arr[1]->head);
    return 0;
}

/*
Deck functions:
    createDeck(int id);                 // Creates deck element and assigns an id to it. Example: Deck *dq = createDeck(0); (use id 0 if not using array)
    checkEmpty(Deck *d);                // Checks if deck is empty (returns 0 if true and -1 if false)
    checkFull(Deck *d);                 // Checks if deck is full (returns 0 if true and -1 if false)
    print(Node *n);                     // Prints deck elements to the screen
    pushFront(Deck *d, int n_data);     // Pushes elements to the front of the deck
    pushBack(Deck *d, int n_data);      // Pushes elements to the back of the deck
    popFront(Deck *d);                  // Pops element from the front of the deck. Example: int x = popFront(dq);
    addToArray(Deck **ptr_array, Deck *ptr, int *elementCount);

Menu functions:
    waitForInput();
    checkID(int idToCheck, int deckCounter, Deck **deck);
    askForDeck(int deckCounter, Deck **deck);

Make int elements variable (int elements = 0;)
Create Deck element and assign it to pointer (Deck *dq = createDeck(0));

*/

/*
Klaida #1: 
    Nepavyko paskirstyti atminties

Klaida #2:
    Nera prieinamu deku

Klaida #3:
    Toks ID neegzistuoja.

Klaida #4:
    Dekas yra tuscias. Nepavyko parodyti elementus.

Klaida #5:
    Nepavyko isimti elementa is deko. Dekas yra tuscias.


*/