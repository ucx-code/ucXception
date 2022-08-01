#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_MEM 4750000
int vizinhos[35][34];
int counter[35];
unsigned char state[MAX_MEM][35];
unsigned char stateOld[MAX_MEM][35];
int stateCounter = 1;
int n;

int testa(unsigned char* estado)
{
    int a;
    for (a = 0;a < n;a++)
        if (estado[a] == 0) 
            return -1;
    return 1;
}

int criaEstados(unsigned char* estado, int nivel)
{
    unsigned char novoEstado[35];
    memcpy(novoEstado, estado, 35 * sizeof (char));
    int i;
    for (i = 0;i < n;i++){
        if (estado[i] == 0) {
            novoEstado[i] = 2;
            int z;
            for (z = 0;z < counter[i];z++)
                novoEstado[vizinhos[i][z]] = 1;
            if (testa(novoEstado) == 1) {
                printf("%d\n", nivel + 1);
                return 1;
            }
            memcpy(state[stateCounter++], novoEstado, sizeof (char) * 35);
            memcpy(novoEstado, estado, 35 * sizeof (char));
        }
    }
    return 0;
}
void dfs(int nivel)
{
    int i;
    memcpy(stateOld, state, sizeof (char) * 35 * MAX_MEM);
    int oldCounter = stateCounter;
    stateCounter = 0;
    for (i = 0;i < oldCounter;i++){
        if (criaEstados(stateOld[i], nivel) == 1) 
            return;
    }
    dfs(nivel + 1);
}

int main()
{
    scanf("%d", &n);
    if (n <= 1) {
        printf("%d\n", n);
        return 0;
    }
    int tmpA, tmpB;
    while (scanf("%d %d", &tmpA, &tmpB) != EOF){
        vizinhos[tmpA - 1][counter[tmpA - 1]] = tmpB - 1;
        vizinhos[tmpB - 1][counter[tmpB - 1]] = tmpA - 1;
        counter[tmpA - 1]++;
        counter[tmpB - 1]++;
    }
    dfs(0);
    return 0;
}
