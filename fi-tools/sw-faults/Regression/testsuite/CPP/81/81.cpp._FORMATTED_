#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
int friendship[2000];
int choices[2000][3];
int lista[200][2][2000];
int listaC[200][2];
int unitClause[200][2][2000];
int unitClauseC[200][2];

void showC(unsigned char* c, int end)
{
    int i;
    for (i = 0;i < end;i++)
        printf("%d", c[i]);
    printf("\n");
}

int n, m;
int best = 99999;

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
int calcB(unsigned char* c, int begin, int max)
{
    int i, a;
    unsigned char estado[2000];
    memset(estado, 0, sizeof (unsigned char) * m);
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
    for (i = begin;i < max;i++){
        for (a = 0;a < listaC[i][0];a++){
            estado[lista[i][0][a]] = 1;
        }
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

int calcB2(unsigned char* c, int begin, int max)
{
    int i, a;
    int soma = 0;
    for (i = begin;i < max;i++){
        int set1 = 0;
        int set2 = 0;
        for (a = 0;a < unitClauseC[0][i];a++)
            set1 += friendship[unitClause[0][i][a]];
        for (a = 0;a < unitClauseC[1][i];a++)
            set2 += friendship[unitClause[1][i][a]];
        soma += (set1 < set2) ? set1 : set2;
    }
    return soma;
}

void rec(unsigned char* c, int begin, int end)
{
    if (begin != end) {
        int actual = calcB(c, begin, end);
        if (actual + calcB2(c, begin, end) > best) 
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

int main()
{
    int i;
    scanf("%d %d", &n, &m);
    for (i = 0;i < m;i++){
        scanf("%d", &friendship[i]);
        int tmp;
        scanf("%d", &tmp);
        int a = 0;
        int k;
        while (tmp != 0){
            if (tmp < 0) 
                lista[abs(tmp) - 1][0][listaC[abs(tmp) - 1][0]++] = i;
            else
                lista[abs(tmp) - 1][1][listaC[abs(tmp) - 1][1]++] = i;

            choices[i][a++] = tmp;
            k = tmp;
            scanf("%d", &tmp);
        }
        if (a == 1) {
            if (k < 0) {
                unitClause[abs(k) - 1][0][unitClauseC[abs(k) - 1][0]++] = i;
            }else{
                unitClause[abs(k) - 1][1][unitClauseC[abs(k) - 1][1]++] = i;
            }
        }
    }
    best = 999999;
    rec(c, 0, n);
    printf("%d\n", best);
}
