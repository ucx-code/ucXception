#include <iostream>
#include <cstdio>
#include <cstdlib>
#define MAX_AMIGOS 200
#define MAX_INGREDIENTES 2000
using namespace std;
int nAmigos, nIngredienteS;
int valor_amizades[MAX_AMIGOS];
int moods[MAX_AMIGOS];
int combinacoes[MAX_INGREDIENTES];
int opinions[MAX_AMIGOS][MAX_INGREDIENTES];
int unhapiness;
int l_unhapiness = MAX_AMIGOS * 1000;
void marca(int x);
void desmarca(int x);
void select_ingredients(int x);

int main()
{
    int i;
    int taste;
    scanf("%d %d\n", &nIngredienteS, &nAmigos);
    for (i = 0;i < nAmigos;i++){
        scanf("%d", &valor_amizades[i]);
        while (true){
            scanf("%d", &taste);
            if (taste == 0) {
                break;
            }
            if (taste < 0) {
                moods[i]++;
                opinions[i][abs(taste) - 1] = -1;
            }else{
                opinions[i][taste - 1] = 1;
            }
        }
        if (moods[i] == 0) {
            unhapiness += valor_amizades[i];
        }
    }
    select_ingredients(-1);
    printf("%d\n", l_unhapiness);
    return 0;
}

void select_ingredients(int x)
{
    int z;
    int i;
    if (x == nIngredienteS) {
        return;
    }
    if (unhapiness < l_unhapiness && unhapiness >= 0) {
        l_unhapiness = unhapiness;
    }
    for (z = x + 1;z < nIngredienteS;z++){
        marca(z);
        select_ingredients(z);
        desmarca(z);
    }
}

void marca(int x)
{
    int i;
    combinacoes[x] = 1;
    for (i = 0;i < nAmigos;i++){
        if (opinions[i][x] == -1) {
            moods[i]--;
        }
        if (opinions[i][x] == 1) {
            moods[i]++;
        }
        if (moods[i] <= 0) {
            unhapiness += valor_amizades[i];
        }
    }
}

void desmarca(int x)
{
    int i;
    combinacoes[x] = 0;
    for (i = 0;i < nAmigos;i++){
        if (opinions[i][x] == -1) {
            moods[i]++;
        }
        if (opinions[i][x] == 1) {
            moods[i]--;
        }
        if (moods[i] > 0) {
            unhapiness -= valor_amizades[i];
        }
    }
}
