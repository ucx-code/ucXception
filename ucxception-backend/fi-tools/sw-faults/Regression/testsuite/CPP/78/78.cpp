#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <queue>
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
int best = 9999999;
int last = best;

int unitPropagation()
{
    int i, a, j, k, z;
    bool tem;
    for (i = 0;i < n;i++){
        for (a = 0;a < unitClauseC[i][1];a++){
            for (j = 0;j < m;j++){
                tem = false;
                for (k = 0;k < 3;k++){
                    if (choices[j][k] == 0) 
                        break;

                    if (choices[j][k] == i + 1) {
                        tem = true;
                        break;
                    }
                }
            }
            if (tem == true) {
                choices[j][0] = 0;
            }
            for (j = 0;j < m;j++){
                for (k = 0;k < 3;k++){
                    if (choices[j][k] == 0) 
                        break;

                    if (choices[j][k] == -(i + 1)) {
                        for (z = k;z < 2;z++){
                            choices[j][z] = choices[j][z + 1];
                        }
                        choices[j][2] = 0;
                    }
                }
            }
        }
    }
    return 1;
}
int underestimation()
{
    int under = 0;
}

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

unsigned char forced[200];

void rec(unsigned char* c, int begin, int end)
{
    if (begin != end) {
        if (forced[begin] == 1) {
            rec(c, begin + 1, end);
            return;
        }
        int actual = calcB(c, begin, end);
        if (actual > best) 
            return;
    }
    if (begin == end) {
        int soma = calc(c, end);
        if (soma == 407) 
            showC(c, end);

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
        int a = 0, k;
        while (tmp != 0){
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
    printf("\n");
    unitPropagation();
    int a;
    for (i = 0;i < m;i++){
        for (a = 0;a < 3;a++){
            if (choices[i][a] == 0) 
                break;

            int tmp = choices[i][a];
            if (tmp < 0) 
                lista[abs(tmp) - 1][0][listaC[abs(tmp) - 1][0]++] = i;
            else
                lista[abs(tmp) - 1][1][listaC[abs(tmp) - 1][1]++] = i;
        }
    }
    printf("\n");
    for (i = 0;i < n;i++){
        if ((listaC[i][1] > 0) && (listaC[i][0] == 0)) 
            forced[i] = 1;
    }
    for (i = 0;i < n;i++){
        int k = unitClauseC[i][1];
        int k2 = listaC[i][0];
        if (k2 <= k) {
            forced[i] = 1;
        }
    }
    showC(forced, n);
    rec(c, 0, n);
    printf("%d\n", best);
}
