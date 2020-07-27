#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int vizinhos[35][34];
int counter[35];
int melhor = 9999;
int estadosP[35][35];

int rec(int pos, int n, int count, int estadoCounter, int limit)
{
    ++count;
    if (count > limit + 1) 
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
            if (rec(i, n, count, estadoCounter + 1, limit) == 1) 
                return 1;
        }
    }
    if ((todosTemAcesso)&&(count < melhor)) {
        melhor = count;
        return 1;
    }
    return -1;
}

int compare(const void* a, const void* b)
{
    return (counter[*(int*)b] + counter[*(int*)a]);
}

void memosSet()
{
    int a;
    for (a = 0;a < 35;a++)
        memset(estadosP[a], 0, sizeof (int) * 35);
}

int main()
{
    int n;
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
    int i;
    int z;
    int zBegin;
    for (z = 0;z <= n / 8;z++)
        for (i = 1;i < n;i++){
            int a;
            memosSet();
            if (rec(i, n, 0, 0, z) == 1) 
                goto fim;
        }
    printf("BANG\n");
    for (z = n / 2;z > n / 8;z--)
        for (i = 1;i < n;i++){
            int a;
            for (a = 0;a < 35;a++)
                memset(estadosP[a], 0, sizeof (int) * 35);
            if (rec(i, n, 0, 0, z) == 1) 
                goto fim;
        }
    fim:
    printf("%d\n", melhor);
    return 0;
}
