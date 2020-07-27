#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
unsigned int friendship[2000];
short choices[2000][3];
unsigned short lista[200][2][2000];
unsigned short listaC[200][2];

void showC(int* c, int end)
{
    int i;
    for (i = 0;i < end;i++)
        printf("%d", c[i]);
    printf("\n");
}

int n, m;
int best = 9999999;

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
    register int i, a;
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
    if (begin == end) {
        int soma = calc(c, end);
        if (soma < best) 
            best = soma;

        return;
    }
    int actual = 0;
    int actual2 = 0;
    c[begin] = 0;
    actual = calcB(c, begin + 1, end);
    c[begin] = 1;
    actual2 = calcB(c, begin + 1, end);
    if (actual2 < best) {
        c[begin] = 1;
        rec(c, begin + 1, end);
    }
    if (actual < best) {
        c[begin] = 0;
        rec(c, begin + 1, end);
    }
}

unsigned char c[200];

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
    precalc(n);
    rec(c, 0, n);
    printf("%d\n", best);
}
