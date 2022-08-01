#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int vizinhos[36][36];
int counter[36];
int visit[36];
int best = 999999;
int M[36][36];
int n;

int getDist(int a, int b, int len)
{
    int i;
    for (i = 0;i < counter[a];i++){
        if (vizinhos[a][i] == b) 
            return len;
    }
    for (i = 0;i < counter[a];i++){
        int size = getDist(vizinhos[a][i], b, len + 1);
        if (size != -1) 
            return size;
    }
    return -1;
}

void criaM()
{
    int i;
    int a;
    for (i = 0;i < n;i++){
        for (a = 0;a < n;a++){
            M[i][a] = getDist(i, a, 1);
        }
    }
}

int soma(int* visit)
{
    int i;
    int count = 0;
    for (i = 1;i <= n;i++)
        if (visit[i] == 1) 
            ++count;
    return count;
}

int saoVizinhos(int i, int j)
{
    int a;
    for (a = 0;a < counter[i];a++)
        if (vizinhos[i][a] == j) 
            return 1;
    return 0;
}
void shortPath(int v, int len)
{
    if (len >= best) 
        return;

    if ((soma(visit) == n) && (len < best)) {
        best = len;
        return;
    }
    int i;
    for (i = 1;i <= n;i++){
        if (saoVizinhos(v, i) == 1) 
            if (visit[i] == 0) {
                visit[i] = 1;
                shortPath(i, len + 1);
                visit[i] = 0;
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
        vizinhos[tmpA][counter[tmpA]] = tmpB;
        vizinhos[tmpB][counter[tmpB]] = tmpA;
        counter[tmpA]++;
        counter[tmpB]++;
    }
    shortPath(1, 0);
    printf("%d\n", best);
    return 0;
}
