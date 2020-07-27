#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int vizinhos[36][36];
int counter[36];
int neighbor[36];
int state[36];
int best = 999;
int n;

int saoVizinhos(int i, int j)
{
    int a;
    for (a = 0;a < counter[i];a++)
        if (vizinhos[i][a] == j) 
            return 1;
    return 0;
}

int chegaPaTodos()
{
    int marca[36];
    int i, a;
    memcpy(marca, state, sizeof (int) * 36);
    for (i = 1;i <= n;i++){
        if (state[i] == 2) {
            for (a = 0;a < counter[i];a++){
                marca[vizinhos[i][a]] = 1;
            }
        }
    }
    for (i = 1;i <= n;i++)
        if (marca[i] == 0) 
            return 0;
    return 1;
}
void mis(int v, int size)
{
    int k = chegaPaTodos();
    if ((k == 1) && (size < best)) {
        best = size;
    }
    int i;
    for (i = v + 1;i <= n;i++){
        if (saoVizinhos(i, v) == 1) {
            neighbor[i] = neighbor[i] + 1;
        }
    }
    for (i = v + 1;i <= n;i++){
        if (neighbor[i] == 0) {
            state[i] = 2;
            mis(i, size + 1);
            state[i] = 0;
        }
    }
    for (i = v + 1;i <= n;i++){
        if (saoVizinhos(i, v) == 1) {
            neighbor[i] = neighbor[i] - 1;
        }
    }
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
        if ((tmpA != tmpB) && (saoVizinhos(tmpA, tmpB) == 0)) {
            vizinhos[tmpA][counter[tmpA]] = tmpB;
            vizinhos[tmpB][counter[tmpB]] = tmpA;
            counter[tmpA]++;
            counter[tmpB]++;
        }
    }
    mis(0, 0);
    printf("%d\n", best);
    return 0;
}
