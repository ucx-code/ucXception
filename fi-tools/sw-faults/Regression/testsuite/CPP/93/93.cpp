#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int vizinhos[502][502];
int vizinhosC[502];
bool marked[502];
bool color[502];
bool is = true;

void dfs(int v)
{
    marked[v] = true;
    int w;
    for (w = 0;w < vizinhosC[v];w++){
        if (marked[vizinhos[v][w]] == false) {
            color[vizinhos[v][w]] = !color[v];
            dfs(vizinhos[v][w]);
        }else
            if (color[vizinhos[v][w]] == color[v]) {
                is = false;
                return;
            }
    }
}

int main()
{
    int n, m;
    int i;
    while (scanf("%d %d", &n, &m) != EOF){
        for (i = 0;i < m;i++){
            int a, b;
            scanf("%d %d", &a, &b);
            vizinhos[a][vizinhosC[a]++] = b;
            vizinhos[b][vizinhosC[b]++] = a;
        }
        is = true;
        memset(marked, 0, sizeof (bool) * n);
        memset(color, 0, sizeof (bool) * n);
        for (i = 0;i < n;i++){
            if (!marked[i]) 
                dfs(i);
        }
        if (is == true) 
            printf("NOT SURE\n");
        else
            printf("NO\n");
    }
    return 0;
}
