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
    {
        // cout << '-';
        outFull << '-';
    }
    // if (addedElement != -1)
        outFull << "Bandoma prideti d" << addedElement+1 << ". Kuprine = "; 
    // cout << "{ ";
    outFull << "{";

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
    if (option == 0)
    {
        // cout << "D" << index << " ";
        outFull << " d" << index;
        totalPrice += items[index - 1].value;
        totalWeight += items[index - 1].weight;
    }

    // cout << "} ";
    outFull << "}. ";
    // cout << "K: " << totalPrice << " ";
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
    if (counter == 1)
        outFull << ". ";
    else
        outFull << "=" << totalPrice << ". ";
    
    // cout << "S: " << totalWeight;
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
    if (counter == 1)
        outFull << ". ";
    else
        outFull << "=" << totalWeight << ". ";

    if (option && knapsack[knapsack.size() - 1] != 1)
    {
        // cout << " - GILYN";
        outFull << " GILYN.";
    }
    else
    {
        if (totalWeight > S)
            outFull << " BACKTRACK, nes svoris virsija maksimuma S = " << S << '.';
        // cout << " - BACKTRACK";
        else
            outFull << " BACKTRACK, parenkama kita kombinacija.";
    }
    // cout << endl;
    outFull << endl;
}

void printAnswer(vector<Item> items, vector<int> knapsack, ofstream &outShort, ofstream &outFull)
{
    int totalPrice = 0;
    int totalWeight = 0;

    // cout << endl << "\t\t{ ";
    // outShort << endl << "\t\t{ ";
    outFull << endl << "\t\t{";
    for (int i = 0; i < knapsack.size(); i++)
    {
        if (knapsack[i] == 1)
        {
            // cout << "D" << i + 1 << " ";
            // outShort << "D" << i + 1 << " ";
            if (i == knapsack.size()-1)
                outFull << "d" << i + 1;
            else
                outFull << "d" << i + 1 << " ";
            totalPrice += items[i].value;
            totalWeight += items[i].weight;
        }
    }

    // cout << "} " << endl;
    // outShort << "} " << endl;
    outFull << "}. " << endl;

    // cout << "\t\tKaina: " << totalPrice << endl;
    // outShort << "\t\tKaina: " << totalPrice << endl;
    outFull << "\t\tKaina = " << totalPrice << '.' << endl;

    // cout << "\t\tSvoris: " << totalWeight;
    // outShort << "\t\tSvoris: " << totalWeight;
    outFull << "\t\tSvoris = " << totalWeight << '.';

    // cout << endl;
    // outShort << endl;
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
        outFull << setw(5) << steps++ << ") ";
        knapsack[index] = 1;
        // cout << "INDEX: " << index << endl;
        printKnapsack(items, knapsack, 1, 0, outFull, index);
        knapsackBacktracking(items, capacity - items[index].weight, knapsack, index + 1, bestKnapsack, bestValue, outFull);
        after = true;
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

    // cout << "Olen Rackauskas, 1 kursas, 3 grupe, 2 pogrupis, 2 uzduotis, 3 variantas" << endl
        // << endl;
    // cout << "Uzduoties aprasas: Duota N daiktu, kuriu svoriai s1, s2, ... sN, o kainos k1, k2, ... kN. Programa turi sudaryti daiktu rinkini, kurio kaina butu maksimali, o svoris nevirsytu nurodyto svorio S. Vartotojas nurodo faila, is kurio programa iveda daiktu svorius ir kainas, bei svori S." << endl
        // << endl;

    string filename;
    cout << "Iveskite failo pavadinima (rasoma su pletiniu gale): ";
    getline(cin, filename);
    ifstream src(filename.c_str());       // open data file
    ofstream outShort("out-trumpas.txt"); // open short output file
    ofstream outFull("out-ilgas.txt");    // open full output file
    if (src)
    {
        cout << "Failas " << filename << " nuskaitytas sekmingai." << endl << endl;
    }
    else
    {
        cout << "Failas " << filename << " nerastas." << endl << endl;
        return 0;
    }

    // outShort << "Olen Rackauskas, 1 kursas, 3 grupe, 2 pogrupis, 2 uzduotis, 3 variantas" << endl
        // << endl;
    outFull << "Olen Rackauskas, 1 kursas, 3 grupe, 2 pogrupis." << endl
        << endl;
    // 2 uzduotis, 3 variantas
    // outShort << "Uzduoties aprasas: Duota N daiktu, kuriu svoriai s1, s2, ... sN, o kainos k1, k2, ... kN. Programa turi sudaryti daiktu rinkini, kurio kaina butu maksimali, o svoris nevirsytu nurodyto svorio S. Vartotojas nurodo faila, is kurio programa iveda daiktu svorius ir kainas, bei svori S." << endl
        // << endl;

    // cout << "Ivesties failas: " << filename << endl << endl;
    // outShort << "Ivesties failas: " << filename << endl << endl;
    outFull << "1 DALIS. DUOMENYS." << endl;
    outFull << "2 uzduotis, 3 variantas" << endl << endl;
    outFull << "1.1. SALYGA. ";
    outFull << "Duota N daiktu, kuriu svoriai s1, s2, ... sN, o kainos k1, k2, ... kN. Programa turi sudaryti daiktu rinkini, kurio kaina butu maksimali, o svoris nevirsytu nurodyto svorio S. Vartotojas nurodo faila, is kurio programa iveda daiktu svorius ir kainas, bei svori S." << endl
        << endl;
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

    // vector<Item> items = {{1,5}, {2,6}, {3,7}, {4,8}, {5,9}};
    // cout << "I DALIS: Duomenys" << endl;
    // outShort << "I DALIS: Duomenys" << endl;
    

    // cout << "\t1)Daiktu skaicius N: " << N << endl;
    // outShort << "\t1)Daiktu skaicius N: " << N << endl;
    outFull << "\t1.2.2)Daiktu skaicius N: " << N << endl;

    // cout << "\t2)Maksimalus daiktu svoris S: " << S << endl;
    // outShort << "\t2)Maksimalus daiktu svoris S: " << S << endl;
    outFull << "\t1.2.3)Maksimalus daiktu svoris S: " << S << endl;

    // cout << "\t3)Daiktu svoriai ir kainos:" << endl;
    // outShort << "\t3)Daiktu svoriai ir kainos:" << endl;
    outFull << "\t1.2.4)Daiktu svoriai ir kainos:" << endl;

    for (int i = 0; i < N; i++)
    {
        // cout << "\t\tD" << i + 1 << ": svoris - " << items[i].weight << "; kaina - " << items[i].value << endl;
        // outShort << "\t\tD" << i + 1 << ": svoris - " << items[i].weight << "; kaina - " << items[i].value << endl;
        outFull << "\t\td" << i + 1 << ". s" << i+1 << " = " << items[i].weight << ". k" << i+1 << " = " << items[i].value << '.' << endl;
    }
    // cout << endl;
    // outShort << endl;
    outFull << endl;
    vector<int> knapsack(items.size(), 0);
    vector<int> bestKnapsack(items.size(), 0);
    int bestValue = 0;

    // cout << "II DALIS:" << endl;
    outFull << "2 DALIS. VYKDYMAS." << endl;
    knapsackBacktracking(items, S, knapsack, 0, bestKnapsack, bestValue, outFull);

    // cout << endl << "III DALIS: Rezultatas" << endl;
    // outShort << endl << "III DALIS: Rezultatas" << endl;
    outFull << endl << "3 DALIS. Rezultatas" << endl;

    int itemCount = 0;
    for (int i = 0; i < bestKnapsack.size(); i++)
    {
        if (bestKnapsack[i] == 1)
            itemCount++;
    }

    if(itemCount > 0)
    {
        // cout << "\t3.1)Daiktu sarasas rastas. Bandymu skaicius: " << steps - 1 << endl;
        // outShort << "\t3.1)Daiktu sarasas rastas. Bandymu skaicius: " << steps - 1 << endl;
        outFull << "\t3.1)Daiktu sarasas rastas. Bandymu skaicius: " << steps - 1 << endl;

        // cout << "\t3.2)Geriausias rastas daiktu rinkinys: ";
        // outShort << "\t3.2)Geriausias rastas daiktu rinkinys: ";
        outFull << "\t3.2)Geriausias rastas daiktu rinkinys: ";
        printAnswer(items, bestKnapsack, outShort, outFull);
    }
    else
    {
        // cout << "\t3.1)Daiktu sarasas nerastas. Bandymu skaicius: " << steps - 1 << endl << "\tNera daiktu, kurie turetu svori mazesni nei " << S << endl;
        // outShort << "\t3.1)Daiktu sarasas nerastas. Bandymu skaicius: " << steps - 1 << endl << "\tNera daiktu, kurie turetu svori mazesni nei " << S << endl;
        outFull << "\t3.1)Daiktu sarasas nerastas. Bandymu skaicius: " << steps - 1 << endl << "\tNera daiktu, kurie turetu svori mazesni nei " << S << endl;
    }

    outShort.close();
    outFull.close();
    return 0;
}
