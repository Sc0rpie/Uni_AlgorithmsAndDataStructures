// Funkcija suma (sveiku) ir dalmuo su liekana
// Pradiniai skaiciai: sveiki skaiciai

#include <stdio.h>

void suma(int a, int b) // Funkcija, kuri priima du sveikus skaicius ir paraso ju suma
{
    int suma;
    suma = a + b;
    printf("Suma: %d\n", suma);
}

void dalmuo(int a, int b) // Funkcija, kuri priima du sveikus skaicius ir paraso ju dalmeni ir liekana
{
    if (b == 0) // Tikrinama ar dalyba is nulio
    {
        printf("Dalyba is nulio negalima!\n");
        return;
    }
    else
    {
        int dalmuo, liekana;
        dalmuo = a / b;
        liekana = a % b;
        printf("Dalmuo: %d\nLiekana: %d\n", dalmuo, liekana);
    }
}

int main()
{
    int a, b;
    printf("Iveskite du sveikus skaicius: ");
    scanf("%d %d", &a, &b);
    suma(a, b);
    dalmuo(a, b);

    return 0;
}