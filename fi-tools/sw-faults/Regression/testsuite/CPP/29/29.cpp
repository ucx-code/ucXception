#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int vizinhos[36][36];
int neighbor[36];
int state[36];
int best = 999;
int n;

int saoVizinhos(int i, int j)
{
    if (j > i) 
        return vizinhos[j][i];
    else
        return vizinhos[i][j];
}

int chegaPaTodos()
{
    int marca[36];
    int i, a;
    memset(marca, 0, sizeof (int) * 36);
    int count = 0;
    int nPrinters = 0;
    for (i = 1;i <= n && count < n;i++){
        if (state[i] == 2) {
            if (marca[i] == 0) {
                marca[i] = 1;
                ++count;
            }
            ++nPrinters;
            for (a = 1;a < i;a++){
                if (vizinhos[a][i] == 1 && marca[a] != 1) {
                    marca[a] = 1;
                    count++;
                }
            }
            for (a = i + 1;a <= n;a++){
                if (vizinhos[i][a] == 1 && marca[a] != 1) {
                    marca[a] = 1;
                    count++;
                }
            }
        }
    }
    if (count < n) 
        return 0;

    if ((nPrinters != 0) && (nPrinters < best)) {
        best = nPrinters;
    }
    return 1;
}
void mis(int v, int size)
{
    if (size > best) 
        return;

    int k = chegaPaTodos();
    if (k == 1) 
        return;

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
        vizinhos[tmpA][tmpB] = 1;
    }
    mis(0, 0);
    printf("%d\n", best);
    return 0;
}
