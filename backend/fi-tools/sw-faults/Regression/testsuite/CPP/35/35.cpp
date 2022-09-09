#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int vizinhos[35][34];
int counter[35];
int melhor = 9999;
int estadosP[35][35];
int translator[35][35];
int n;

int countVizinhos(int pos, int n, int estadoCounter)
{
    int count = 0;
    int i;
    for (i = 0;i < counter[pos];i++){
        if (estadosP[estadoCounter][vizinhos[pos][i]] == 0) 
            ++count;
    }
    return count;
}

int rec(int pos, int n, int count, int estadoCounter)
{
    ++count;
    if (count > n / 2) 
        return -1;

    if (count >= melhor) 
        return -1;

    estadosP[estadoCounter][pos] = 2;
    int i;
    for (i = 0;i < counter[pos];i++){
        estadosP[estadoCounter][vizinhos[pos][i]] = 1;
    }
    bool todosTemAcesso = true;
    for (i = 0;i < n;i++){
        if (estadosP[estadoCounter][i] == 0) {
            todosTemAcesso = false;
            memcpy(estadosP[estadoCounter + 1], estadosP[estadoCounter], sizeof (int) * 35);
            rec(i, n, count, estadoCounter + 1);
        }
    }
    if ((todosTemAcesso)&&(count < melhor)) 
        melhor = count;

    return 1;
}

int compare(const void* a, const void* b)
{
    return countVizinhos(translator[0][*(int*)b], n, 0) - countVizinhos(translator[0][*(int*)a], n, 0);
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
    int i, a;
    for (a = 0;a < n;a++)
        for (i = 0;i < n;i++)
            translator[a][i] = i;
    qsort(translator[0], n, sizeof (int), compare);
    int maxi = 1;
    for (i = 1;i < n;i++)
        if (countVizinhos(translator[0][i], n, 0) == countVizinhos(translator[0][0], n, 0)) 
            ++maxi;
    for (i = 0;i < maxi;i++){
        int a;
        for (a = 0;a < 35;a++)
            memset(estadosP[a], 0, sizeof (int) * 35);
        rec(translator[0][i], n, 0, 0);
    }
    printf("%d\n", melhor);
    return 0;
}
