#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int vizinhos[600][600];
int vizinhosC[600];
int color[600];

bool dfs(int node, int c)
{
    if ((color[node] == c) || (color[node] == -c)) {
        if (color[node] == c) {
            return true;
        }else{
            return false;
        }
    }
    color[node] = c;
    for (int i = 0;i < vizinhosC[node];i++)
        if (dfs(vizinhos[node][i], -c) == false) {
            return false;
        }
    return true;
}

void readLine(int* a, int* b)
{
    *a = 0;
    *b = 0;
    int tmp = getchar_unlocked();
    while (tmp != ' '){
        *a = *a * 10 + (tmp - 48);
        tmp = getchar_unlocked();
    }
    tmp = getchar_unlocked();
    while (tmp != '\n'){
        *b = *b * 10 + (tmp - 48);
        tmp = getchar_unlocked();
    }
}

int main()
{
    int n, m;
    int i;
    int it = 0;
    while (scanf("%d %d", &n, &m) != EOF){
        for (i = 0;i < m;i++){
            int a, b;
            scanf("%d %d", &a, &b);
            vizinhos[a][vizinhosC[a]++] = b;
            vizinhos[b][vizinhosC[b]++] = a;
        }
        ++it;
        if (dfs(1, it) == true) 
            printf("NOT SURE\n");
        else
            printf("NO\n");

        memset(vizinhosC, 0, sizeof (int) * (n + 1));
    }
    return 0;
}
