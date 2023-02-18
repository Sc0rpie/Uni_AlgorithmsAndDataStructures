// Yra 3 taskai (x1,y1); (x2,y2); (x3,y3). Patikrinti ar taskai sudaro statuji trikampi.
// Rezultatas: sudaro arba nesudaro
// Atsakymas: Yra arba nera (kitas tekstas...)

#include <stdio.h>
#include <math.h>

int atstumas(int x1, int y1, int x2, int y2)
{
    return (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1);
}

void arStatus(float A, float B, float C)
{
    if (A > 0 && B > 0 && C > 0)
    {
        if (A + B == C)
            printf("Trikampis yra status pagal pitagoro teorema: A^2 + B^2 = C^2\n");
        else if (A + C == B)
            printf("Trikampis yra status pagal pitagoro teorema: A^2 + C^2 = B^2\n");
        else if (B + C == A)
            printf("Trikampis yra status pagal pitagoro teorema: B^2 + C^2 = A^2\n");
        else
            printf("Trikampis nera status, nes liniju A, B ir C atstumai neatitinka pitagoro teoremos: A^2 + B^2 = C^2, ...\n");
    }
    else
    {
        if (A == 0)
            printf("Klaida: A linijos atstumas lygus 0.\nPriezastis: 1 ir 2 taskai nesudaro tieses.\n");
        else if (B == 0)
            printf("Klaida: B linijos atstumas lygus 0.\nPriezastis: 1 ir 3 taskai nesudaro tieses.\n");
        else
            printf("Klaida: C linijos atstumas lygus 0.\nPriezastis: 2 ir 3 taskai nesudaro tieses.\n");
    }
}

int main()
{
    int x1, x2, x3;
    int y1, y2, y3;
    int A, B, C;

    printf("Iveskite pirmojo tasko X koordinate, paspauskite tarpa, iveskite Y koordinate ir paspauskite ENTER. (Skaiciai yra sveikieji)\n");
    scanf("%d %d", &x1, &y1);
    printf("Iveskite antrojo tasko X koordinate, paspauskite tarpa, iveskite Y koordinate ir paspauskite ENTER. (Skaiciai yra sveikieji)\n");
    scanf("%d %d", &x2, &y2);
    printf("Iveskite treciojo tasko X koordinate, paspauskite tarpa, iveskite Y koordinate ir paspauskite ENTER. (Skaiciai yra sveikieji)\n");
    scanf("%d %d", &x3, &y3);
    printf("\n");
    A = atstumas(x1, y1, x2, y2); // Atstumas tarp 1 ir 2 tasko
    B = atstumas(x1, y1, x3, y3); // Atstumas tarp 1 ir 3 tasko
    C = atstumas(x2, y2, x3, y3); // Atstumas tarp 2 ir 3 tasko

    // printf("A = %f\nB = %f\nC = %f\n", A, B, C);
    // printf("asqrt = %d\nbsqrt = %d\ncsqrt = %d\n", asqrt, bsqrt, csqrt);
    if ((x1 == x2 && x1 == x3) && (y1 == y2 && y1 == y3))
    {
        printf("Klaida: Visi taskai yra vienoje tieseje.");
        return 0;
    }
    else if (x1 == x2 && x1 == x3)
    {
        printf("Klaida: Visi X taskai yra vienoje tieseje.");
        return 0;
    }
    else if (y1 == y2 && y1 == y3)
    {
        printf("Klaida: Visi Y taskai yra vienoje tieseje.");
        return 0;
    }

    arStatus(A, B, C);
    printf("\nPrograma baige darba.");
    return 0;
}