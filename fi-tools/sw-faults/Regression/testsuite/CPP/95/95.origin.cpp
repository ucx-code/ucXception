#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MaxN 502

bool bipartite = true;
int gr[MaxN][MaxN], color[MaxN];
bool mark[MaxN];
int n;

bool dfs(int node, int c) {
    if(color[node] != 0) {
        if(color[node] == c) {
            return true;
        } else {
            return false;
        }
    }
    color[node] = c;
    for(int i = 1; i <= n; i++) if(gr[node][i] == 1) {
        if(!dfs(i, -c)) {
            return false;
        }
    }
    return true;
}

int main()
{
     int m; int i;
     while (scanf("%d %d", &n, &m) != EOF)
     {
          for (i = 0; i < m; i++)
          {
               int a2, b2;
               scanf("%d %d", &a2, &b2);
               gr[a2][b2] = 1;
               gr[b2][a2] = 1;
               //vizinhos[a][vizinhosC[a]++] = b;
               //vizinhos[b][vizinhosC[b]++] = a;
          }
          bipartite = false;
          for (i = 0; i < n; i++)   
          {
                         memset(mark, false, sizeof(bool) * 500);
          memset(color, 0, sizeof(int) * 500);       
           if (dfs(i,1) == true) bipartite = true;
          }
          for (i = 0; i <= n; i++)
               memset(gr[i], 0, sizeof(int) * 500);
          if (bipartite == true) printf("NOT SURE\n");
               else printf("NO\n");    
     }
     
     return 0;
}
