#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int vizinhos[35][34];
int counter[35];
int melhor = 9999;
int estadosP[35];

int rec(int pos, int n, int count, int* estado)
{
    ++count;
    if (count > n / 2) 
        return -1;

    estado[pos] = 2;
    int i;
    for (i = 0;i < counter[pos];i++){
        estado[vizinhos[pos][i]] = 1;
    }
    bool todosTemAcesso = true;
    for (i = 0;i < n;i++){
        if (estado[i] <= 0) {
            todosTemAcesso = false;
            rec(i, n, count, estado);
        }
    }
    if ((todosTemAcesso)&&(count < melhor)) 
        melhor = count;

    for (i = 0;i < counter[pos];i++){
        estado[vizinhos[pos][i]] = 0;
    }
    estado[pos] = 0;
    return 1;
}

int main()
{
    int estado[35];
    int n;
    scanf("%d", &n);
    if (n == 1) {
        printf("1\n");
        return 0;
    }
    int tmpA, tmpB;
    while (scanf("%d %d", &tmpA, &tmpB) != EOF){
        vizinhos[tmpA - 1][counter[tmpA - 1]] = tmpB - 1;
        vizinhos[tmpB - 1][counter[tmpB - 1]] = tmpA - 1;
        counter[tmpA - 1]++;
        counter[tmpB - 1]++;
    }
    int i;
    for (i = 0;i < n;i++){
        memset(estado, 0, sizeof (int) * 35);
        rec(i, n, 0, estado);
    }
    printf("%d\n", melhor);
    return 0;
}
