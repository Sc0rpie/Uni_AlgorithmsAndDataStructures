#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

// Struct to represent an item
struct Item {
    int value, weight;
};
int steps = 1;
// Function to print the items in the knapsack, as well as the total price and weight of the knapsack
// Option: 0 - backtrack, 1 - gilyn
void printKnapsack(vector<Item> items, vector<int> knapsack, int option, int index, ofstream &outFull) {
    int totalPrice = 0;
    int totalWeight = 0;
    int size = knapsack.size() - count(knapsack.begin(), knapsack.end(), 0);
    // cout << "SIZE: " << size << endl;
    if (option == 0)
        size++;
    for (int i = 0; i < size; i++)
    {
        // cout << '-';
        outFull << '-';
    }
    
    // cout << "{ ";
    outFull << "{ ";
    for (int i = 0; i < knapsack.size(); i++) {
        if (knapsack[i] == 1) {
            // cout << "D" << i+1 << " ";
            outFull << "D" << i+1 << " ";
            totalPrice += items[i].value;
            totalWeight += items[i].weight;
        }
    }
    if (option == 0)
    {
        // cout << "D" << index << " ";
        outFull << "D" << index << " ";
        totalPrice += items[index-1].value;
        totalWeight += items[index-1].weight;
    }
        
    // cout << "} ";
    outFull << "} ";
    // cout << "K: " << totalPrice << " ";
    outFull << "K: " << totalPrice << " ";
    // cout << "S: " << totalWeight;
    outFull << "S: " << totalWeight;
    if (option && knapsack[knapsack.size()-1] != 1)
    {
        // cout << " - GILYN";
        outFull << " - GILYN";
    }
    else
    {
        // cout << " - BACKTRACK";
        outFull << " - BACKTRACK";
    }
    // cout << endl;
    outFull << endl;
}

void printAnswer(vector<Item> items, vector<int> knapsack, ofstream &outShort, ofstream &outFull) {
    int totalPrice = 0;
    int totalWeight = 0;
    
    cout << endl << "\t{ ";
    outShort << endl << "\t{ ";
    outFull << endl << "\t{ ";
    for (int i = 0; i < knapsack.size(); i++) {
        if (knapsack[i] == 1) {
            cout << "D" << i+1 << " ";
            outShort << "D" << i+1 << " ";
            outFull << "D" << i+1 << " ";
            totalPrice += items[i].value;
            totalWeight += items[i].weight;
        }
    }
        
    cout << "} " << endl;
    outShort << "} " << endl;
    outFull << "} " << endl;

    cout << "\tKaina: " << totalPrice << endl;
    outShort << "\tKaina: " << totalPrice << endl;
    outFull << "\tKaina: " << totalPrice << endl;

    cout << "\tSvoris: " << totalWeight;
    outShort << "\tSvoris: " << totalWeight;
    outFull << "\tSvoris: " << totalWeight;

    cout << endl;
    outShort << endl;
    outFull << endl;
}

// Function to calculate the value of the knapsack
int calculateKnapsackValue(vector<Item> items, vector<int> knapsack) {
    int value = 0;
    for (int i = 0; i < knapsack.size(); i++) {
        if (knapsack[i] == 1) {
            value += items[i].value;
        }
    }
    return value;
}

// Recursive function to find the best combination of items
void knapsackBacktracking(vector<Item> items, int capacity, vector<int> knapsack, int index, vector<int>& bestKnapsack, int& bestValue, ofstream &outFull) {
    // If all items have been considered, update the best solution if necessary
    bool after = false;
    if (index == items.size()) {
        int value = calculateKnapsackValue(items, knapsack);
        if (value == 0)
            return;
            
        // printKnapsack(items,knapsack, false);
        if (value > bestValue) {
            bestValue = value;
            bestKnapsack = knapsack;
        }
        return;
    }
    // Check if adding the current item to the knapsack is feasible
    if (items[index].weight <= capacity) {
        outFull << "\t" << steps++ << ") ";
        knapsack[index] = 1;
        // cout << "Checking item " << index+1 << " (value: " << items[index].value << ", weight: " << items[index].weight << ") - added to knapsack" << endl;
        printKnapsack(items, knapsack, 1, 0, outFull);
        knapsackBacktracking(items, capacity - items[index].weight, knapsack, index + 1, bestKnapsack, bestValue, outFull);
        after = true;
        knapsack[index] = 0;
    }

    // Check if not adding the current item to the knapsack is feasible
    // cout << "Step " << steps++ << endl;
    // cout << "Checking item " << index+1 << " (value: " << items[index].value << ", weight: " << items[index].weight << ") - not added to knapsack" << endl;
    if (!after || index == items.size())
    {
        outFull << "\t" << steps++ << ") ";
        printKnapsack(items, knapsack, 0, index+1, outFull);
    }  
    knapsackBacktracking(items, capacity, knapsack, index + 1, bestKnapsack, bestValue, outFull);
}

int main() {
    cout << "Olen Rackauskas, 1 kursas, 3 grupe, 2 pogrupis, 2 uzduotis, 3 variantas" << endl << endl;
    cout << "Uzduoties aprasas: Duota N daiktu, kuriu svoriai s1, s2, ... sN, o kainos k1, k2, ... kN. Programa turi sudaryti daiktu rinkini, kurio kaina butu maksimali, o svoris nevirsytu nurodyto svorio S. Vartotojas nurodo faila, is kurio programa iveda daiktu svorius ir kainas, bei svori S." << endl << endl;

    string filename;
    cout << "Iveskite failo pavadinima (rasoma su .txt gale): ";
    getline(cin, filename);
    ifstream src(filename.c_str());
    ofstream outShort("out-trumpas.txt");
    ofstream outFull("out-ilgas.txt");
    if (src)
    {
        cout << "Failas " << filename << " nuskaitytas sekmingai." << endl;
    }
    else
    {
        cout << "Failas " << filename << " nerastas." << endl;
        return 0;
    }

    cout << "Ivesties failas: " << filename << endl << endl;

    int N, S;
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
    int capacity = S;
    cout << "I DALIS:" << endl;
    outShort << "I DALIS:" << endl;
    outFull << "I DALIS:" << endl;

    cout << "\t1)Daiktu skaicius N: " << N << endl;
    outShort << "\t1)Daiktu skaicius N: " << N << endl;
    outFull << "\t1)Daiktu skaicius N: " << N << endl;

    cout << "\t2)Maksimalus daiktu svoris S: " << S << endl;
    outShort << "\t2)Maksimalus daiktu svoris S: " << S << endl;
    outFull << "\t2)Maksimalus daiktu svoris S: " << S << endl;

    cout << "\t3)Daiktu svoriai ir kainos:" << endl;
    outShort << "\t3)Daiktu svoriai ir kainos:" << endl;
    outFull << "\t3)Daiktu svoriai ir kainos:" << endl;
    for (int i = 0; i < N; i++)
    {
        cout << "\t\tD" << i+1 << ": svoris - " << items[i].weight << "; kaina - " << items[i].value << endl;
        outShort << "\t\tD" << i+1 << ": svoris - " << items[i].weight << "; kaina - " << items[i].value << endl;
        outFull << "\t\tD" << i+1 << ": svoris - " << items[i].weight << "; kaina - " << items[i].value << endl;
    }
    cout << endl;
    outShort << endl;
    outFull << endl;
    vector<int> knapsack(items.size(), 0);
    vector<int> bestKnapsack(items.size(), 0);
    int bestValue = 0;

    // cout << "Knapsack capacity: " << capacity << endl;

    // cout << "II DALIS:" << endl;
    outFull << "II DALIS:" << endl;
    knapsackBacktracking(items, capacity, knapsack, 0, bestKnapsack, bestValue, outFull);

    cout << "III DALIS:" << endl;
    outShort << "III DALIS:" << endl;
    outFull << "III DALIS:" << endl;

    cout << "Daiktu sarasas rastas. Bandymu: " << steps-1 << endl;
    outShort << "Daiktu sarasas rastas. Bandymu: " << steps-1 << endl;
    outFull << "Daiktu sarasas rastas. Bandymu: " << steps-1 << endl;

    cout << "Geriausias rastas daiktu rinkinys: ";
    outShort << "Geriausias rastas daiktu rinkinys: ";
    outFull << "Geriausias rastas daiktu rinkinys: ";
    printAnswer(items,bestKnapsack, outShort, outFull);

    return 0;
}
