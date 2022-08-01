#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int vizinhos[36][36];
int counter[36];
int visit[36];
int best = 999999;
int n;

int verifica(int end)
{
    int count = 0;
    int tmpBest = 0;
    int i, a;
    int tmp[36];
    memset(tmp, 0, sizeof (int) * 36);
    for (i = 0;i < end && count < end;i++){
        if (visit[i] == 1) {
            ++tmpBest;
            if (tmp[i] == 0) {
                tmp[i] = 1;
                ++count;
            }
            for (a = 0;a < i;a++){
                if (vizinhos[a][i] == 1 && tmp[a] != 1) {
                    tmp[a] = 1;
                    count++;
                }
            }
            for (a = i + 1;a < end;a++){
                if (vizinhos[i][a] == 1 && tmp[a] != 1) {
                    tmp[a] = 1;
                    count++;
                }
            }
        }
    }
    if (count < end) 
        return -1;

    if ((tmpBest < best) && (tmpBest != 0)) {
        best = tmpBest;
    }
}

void shortPath(int begin, int end, int len)
{
    if (len > best) 
        return;

    if (verifica(end) == 0) {
        return;
    }
    int i;
    for (i = begin;i < end;i++){
        if (visit[i] == 0) {
            visit[i] = 1;
            shortPath(i, end, len + 1);
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
        vizinhos[tmpA - 1][tmpB - 1] = 1;
    }
    shortPath(0, n, 1);
    printf("%d\n", best);
    return 0;
}
