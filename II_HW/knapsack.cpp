#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>

using namespace std;

// Struct to represent an item
struct Item
{
    int value, weight;
};
int steps = 1;
int S;
// Function to print the items in the knapsack, as well as the total price and weight of the knapsack
// Option: 0 - backtrack, 1 - gilyn
void printKnapsack(vector<Item> items, vector<int> knapsack, int option, int index, ofstream &outFull, int addedElement)
{
    int totalPrice = 0;
    int totalWeight = 0;
    int size = knapsack.size() - count(knapsack.begin(), knapsack.end(), 0);

    if (option == 0)
        size++;
    for (int i = 0; i < size-1; i++)
        outFull << '-';

    outFull << "Bandoma prideti d" << addedElement+1 << ". Kuprine = "; 
    outFull << "{";

    // Printing items in the knapsack (Kuprine = {...})
    int counter = 0;
    for (int i = 0; i < knapsack.size(); i++)
    {
        if (knapsack[i] == 1)
        {
            if (counter != 0)
                outFull << " ";
            outFull << "d" << i + 1;
            totalPrice += items[i].value;
            totalWeight += items[i].weight;
            counter++;
        }
    }
    // Printing the last item in the knapsack if backtracking
    if (option == 0)
    {
        outFull << " d" << index;
        totalPrice += items[index - 1].value;
        totalWeight += items[index - 1].weight;
    }
    outFull << "}. ";

    // Printing th total price of the knapsack (Kaina = ...+...+...=...)
    outFull << "Kaina = ";
    counter = 0;
    for (int i = 0; i < knapsack.size(); i++)
    {
        if(knapsack[i] == 1)
        {
            if (counter != 0)
                outFull << "+";
            outFull << items[i].value;
            counter++;
        }
    }
    // If backtracking (option == 0), print the last item as well
    if (option == 0)
    {
        outFull << '+' << items[index - 1].value;
    }
    // If there is only one item in the knapsack, print a dot instead of the equal sign
    // else print the equal sign and the total price
    if (counter == 1)
        outFull << ". ";
    else
        outFull << "=" << totalPrice << ". ";
    
    // Printing the total weight of the knapsack (Svoris = ...+...+...=...)
    outFull << "Svoris = ";
    counter = 0;
    for (int i = 0; i < knapsack.size(); i++)
    {
        if(knapsack[i] == 1)
        {
            if (counter != 0)
                outFull << "+";
            outFull << items[i].weight;
            counter++;
        }
    }
    // If backtracking (option == 0), print the last item as well
    if (option == 0)
    {
        outFull << '+' << items[index - 1].weight;
    }
    // If there is only one item in the knapsack, print a dot instead of the equal sign
    // else print the equal sign and the total weight
    if (counter == 1)
        outFull << ". ";
    else
        outFull << "=" << totalWeight << ". ";

    // If its going deeper in the tree and last knapsack item is not added, print "GILYN"
    if (option && knapsack[knapsack.size() - 1] != 1)
    {
        outFull << " GILYN.";
    }
    else
    {
        // If the total weight of the knapsack is greater than the capacity of the knapsack, print "BACKTRACK"
        if (totalWeight > S)
            outFull << " BACKTRACK, nes svoris virsija maksimuma S = " << S << '.';
        else
            outFull << " BACKTRACK, parenkama kita kombinacija.";
    }
    outFull << endl;
}

void printAnswer(vector<Item> items, vector<int> knapsack, ofstream &outFull)
{
    int totalPrice = 0;
    int totalWeight = 0;

    outFull << endl << "\t\t{";
    for (int i = 0; i < knapsack.size(); i++)
    {
        if (knapsack[i] == 1)
        {
            if (i == knapsack.size()-1)
                outFull << "d" << i + 1;
            else
                outFull << "d" << i + 1 << " ";
            totalPrice += items[i].value;
            totalWeight += items[i].weight;
        }
    }

    outFull << "}. " << endl;
    outFull << "\t\tKaina = " << totalPrice << '.' << endl;
    outFull << "\t\tSvoris = " << totalWeight << '.';
    outFull << endl;
}

// Function to calculate the value of the knapsack
int calculateKnapsackValue(vector<Item> items, vector<int> knapsack)
{
    int value = 0;
    for (int i = 0; i < knapsack.size(); i++)
    {
        if (knapsack[i] == 1)
        {
            value += items[i].value;
        }
    }
    return value;
}

// Recursive function to find the best combination of items
void knapsackBacktracking(vector<Item> items, int capacity, vector<int> knapsack, int index, vector<int> &bestKnapsack, int &bestValue, ofstream &outFull)
{
    // If all items have been considered, update the best solution if necessary
    bool after = false;
    if (index == items.size())
    {
        int value = calculateKnapsackValue(items, knapsack);
        if (value == 0)
            return;

        // printKnapsack(items,knapsack, false);
        if (value > bestValue)
        {
            bestValue = value;
            bestKnapsack = knapsack;
        }
        return;
    }
    // Check if adding the current item to the knapsack is feasible
    if (items[index].weight <= capacity)
    {
        // Print the current step
        outFull << setw(5) << steps++ << ") ";

        // Current item is being added into a knapsack
        knapsack[index] = 1;

        // Print the current knapsack
        printKnapsack(items, knapsack, 1, 0, outFull, index);

        // Recursively call the function to add the next item
        knapsackBacktracking(items, capacity - items[index].weight, knapsack, index + 1, bestKnapsack, bestValue, outFull);

        // Boolean for removing repeated printKnapsack calls
        after = true;

        // Current item is being removed from the knapsack
        knapsack[index] = 0;
    }

    // Check if not adding the current item to the knapsack is feasible
    if (!after || index == items.size())
    {
        outFull << setw(5) << steps++ << ") ";
        printKnapsack(items, knapsack, 0, index + 1, outFull, index);
    }
    knapsackBacktracking(items, capacity, knapsack, index + 1, bestKnapsack, bestValue, outFull);
}

int main()
{
    string filename;
    cout << "Iveskite failo pavadinima (rasoma su pletiniu gale): ";
    getline(cin, filename);               // get filename from user
    ifstream src(filename.c_str());       // open data file
    if (src)
    {
        cout << "Failas " << filename << " nuskaitytas sekmingai." << endl << endl;
    }
    else
    {
        cout << "Failas " << filename << " nerastas." << endl << endl;
        return 0;
    }
    // Open output file after checking if the input file exists
    ofstream outFull("out-ilgas.txt");    // open full output file


    // Print the header of the output file
    outFull << "Olen Rackauskas, 1 kursas, 3 grupe, 2 pogrupis." << endl << endl;
    outFull << "1 DALIS. DUOMENYS." << endl;
    outFull << "2 uzduotis, 3 variantas" << endl << endl;
    outFull << "1.1. SALYGA. ";
    outFull << "Duota N daiktu, kuriu svoriai s1, s2, ... sN, o kainos k1, k2, ... kN. Programa turi sudaryti daiktu rinkini, kurio kaina butu maksimali, o svoris nevirsytu nurodyto svorio S. Vartotojas nurodo faila, is kurio programa iveda daiktu svorius ir kainas, bei svori S." << endl << endl;
    outFull << "1.2. PRADINE BUSENA." << endl;
    outFull << "\t1.2.1)Ivesties failas: " << filename << endl;

    int N;
    Item item;
    vector<Item> items;
    src >> N;
    src >> S;
    for (int i = 0; i < N; i++)
    {
        src >> item.weight;
        src >> item.value;
        items.push_back(item);
    }
    src.close();

    outFull << "\t1.2.2)Daiktu skaicius N: " << N << endl;
    outFull << "\t1.2.3)Maksimalus daiktu svoris S: " << S << endl;
    outFull << "\t1.2.4)Daiktu svoriai ir kainos:" << endl;

    // Print out the items (d1. s1 = ... k1 = ...)
    for (int i = 0; i < N; i++)
        outFull << "\t\td" << i + 1 << ". s" << i+1 << " = " << items[i].weight << ". k" << i+1 << " = " << items[i].value << '.' << endl;

    outFull << endl;

    // Initialize vectors with size N, filled with 0
    vector<int> knapsack(items.size(), 0);
    vector<int> bestKnapsack(items.size(), 0);
    int bestValue = 0;

    outFull << "2 DALIS. VYKDYMAS." << endl;
    knapsackBacktracking(items, S, knapsack, 0, bestKnapsack, bestValue, outFull);

    outFull << endl << "3 DALIS. Rezultatas" << endl;

    int itemCount = 0;
    for (int i = 0; i < bestKnapsack.size(); i++)
    {
        if (bestKnapsack[i] == 1)
            itemCount++;
    }

    if(itemCount > 0)
    {
        outFull << "\t3.1)Daiktu sarasas rastas. Bandymu skaicius: " << steps - 1 << endl;
        outFull << "\t3.2)Geriausias rastas daiktu rinkinys: ";
        printAnswer(items, bestKnapsack, outFull);
    }
    else
    {
        outFull << "\t3.1)Daiktu sarasas nerastas. Bandymu skaicius: " << steps - 1 << endl << "\tNera daiktu, kurie turetu svori mazesni nei " << S << endl;
    }

    outFull.close();
    return 0;
}
