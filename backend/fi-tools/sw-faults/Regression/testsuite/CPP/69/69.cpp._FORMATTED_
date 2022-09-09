#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
int friendship[2000];
int choices[2000][3];
int lista[200][2][2000];
int listaC[200][2];
int preCalculated[200][2000];

void showC(int* c, int end)
{
    int i;
    for (i = 0;i < end;i++)
        printf("%d", c[i]);
    printf("\n");
}

int n, m;
int best = 9999999;
int estadoLast[2000];

int desmarca(int* c, int i)
{
    int a;
    if (c[i] == 0) 
        for (a = 0;a < listaC[i][0];a++){
            estadoLast[lista[i][0][a]]--;
        }
    else
        for (a = 0;a < listaC[i][1];a++){
            estadoLast[lista[i][1][a]]--;
        }
}
int marca(int* c, int i)
{
    int a;
    if (c[i] == 0) 
        for (a = 0;a < listaC[i][0];a++){
            estadoLast[lista[i][0][a]]++;
        }
    else
        for (a = 0;a < listaC[i][1];a++){
            estadoLast[lista[i][1][a]]++;
        }
}

int calc(int* c)
{
    int i, soma = 0;
    for (i = 0;i < m;i++){
        if (estadoLast[i] == 0) 
            soma += friendship[i];
    }
    return soma;
}

int calcB(int* c, int begin, int max)
{
    int i, a;
    int soma = 0;
    for (i = 0;i < m;i++){
        if ((estadoLast[i] == 0) && (preCalculated[begin - 1][i] == 0)) 
            soma += friendship[i];
    }
    return soma;
}

void rec(int* c, int begin, int end)
{
    int soma = calc(c);
    if (soma < best) {
        best = soma;
    }
    int i;
    if ((begin == -1) || (calcB(c, begin + 1, end) < best)) {
        for (i = begin + 1;i < end;i++){
            c[i] = 1;
            marca(c, i);
            rec(c, i, end);
            desmarca(c, i);
            c[i] = 0;
        }
    }
}

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

int c[200];

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
    rec(c, -1, n);
    printf("%d\n", best);
}
