#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <queue>
int friendship[2000];
int choices[2000][3];
int lista[200][2][2000];
int listaC[200][2];

void showC(int* c, int end)
{
    int i;
    for (i = 0;i < end;i++)
        printf("%d", c[i]);
    printf("\n");
}

int n, m;
int best = 9999999;
int last = best;

int calc(unsigned char* c, int max)
{
    int i, a;
    unsigned char estado[2000];
    memset(estado, 0, sizeof (unsigned char) * m);
    for (i = 0;i < max;i++){
        if (c[i] == 0) 
            for (a = 0;a < listaC[i][0];a++){
                estado[lista[i][0][a]] = 1;
            }
        else
            for (a = 0;a < listaC[i][1];a++){
                estado[lista[i][1][a]] = 1;
            }
    }
    int soma = 0;
    for (i = 0;i < m;i++){
        if (estado[i] == 0) 
            soma += friendship[i];
    }
    return soma;
}
unsigned char preCalculated[200][2000];

void precalc(int max)
{
    int i, a, j;
    for (i = 0;i < max;i++){
        for (j = i + 1;j < max;j++){
            for (a = 0;a < listaC[j][0];a++){
                preCalculated[i][lista[j][0][a]] = 1;
            }
            for (a = 0;a < listaC[j][1];a++){
                preCalculated[i][lista[j][1][a]] = 1;
            }
        }
    }
}

int calcB(unsigned char* c, int begin, int max)
{
    int i, a;
    unsigned char estado[2000];
    memcpy(estado, preCalculated[begin - 1], sizeof (unsigned char) * m);
    for (i = 0;i < begin;i++){
        if (c[i] == 0) 
            for (a = 0;a < listaC[i][0];a++){
                estado[lista[i][0][a]] = 1;
            }
        else
            for (a = 0;a < listaC[i][1];a++){
                estado[lista[i][1][a]] = 1;
            }
    }
    int soma = 0;
    for (i = 0;i < m;i++){
        if (estado[i] == 0) 
            soma += friendship[i];
    }
    return soma;
}

void rec(unsigned char* c, int begin, int end)
{
    if (begin != end) {
        int actual = calcB(c, begin, end);
        if (actual > best) 
            return;
    }
    if (begin == end) {
        int soma = calc(c, end);
        if (soma < best) 
            best = soma;

        return;
    }
    c[begin] = 0;
    rec(c, begin + 1, end);
    c[begin] = 1;
    rec(c, begin + 1, end);
}

unsigned char c[200];
unsigned char c2[200];

int compare(const void* a, const void* b)
{
    return ((*(int*)a) - (*(int*)b));
}

int friendPerIngredient[200];

int main()
{
    int i;
    scanf("%d %d", &n, &m);
    for (i = 0;i < m;i++){
        scanf("%d", &friendship[i]);
        int tmp;
        scanf("%d", &tmp);
        int a = 0;
        while (tmp != 0){
            if (tmp < 0) 
                lista[abs(tmp) - 1][0][listaC[abs(tmp) - 1][0]++] = i;
            else
                lista[abs(tmp) - 1][1][listaC[abs(tmp) - 1][1]++] = i;

            choices[i][a++] = tmp;
            scanf("%d", &tmp);
        }
    }
    unsigned char tmpEstado[2000];
    int a;
    for (i = 0;i < n;i++){
        memset(tmpEstado, 0, sizeof (unsigned char) * m);
        for (a = 0;a < listaC[i][0];a++){
            tmpEstado[lista[i][0][a]] = 1;
        }
        for (a = 0;a < listaC[i][1];a++){
            tmpEstado[lista[i][1][a]] = 1;
        }
        friendPerIngredient[i] = 0;
        for (a = 0;a < m;a++)
            if (tmpEstado[a] == 0) 
                friendPerIngredient[i] += friendship[a];
    }
    memcpy(c2, c, sizeof (unsigned char) * n);
    qsort(c2, n, sizeof (unsigned char), compare);
    precalc(n);
    rec(c, 0, n);
    printf("%d\n", best);
}
