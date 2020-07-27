#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MaxN 502

bool bipartite = true;
int a[MaxN][MaxN], color[MaxN];
bool mark[MaxN];
int n;

void dfs(int v, int cur){
  mark[v] = true;
  color[v] = cur; // color this vertex as cur
  for (int i = 0; i < n; i++)
     if (a[v][i] == 1){ // if there is edge between v and i
         if (color[i] == cur) { // if color of vertex i is equal to color of v, that is cur
               bipartite = false; // graph is definitely not bipartite, so return
               return;
         }
         if (mark[i]==false) dfs(i, cur==1?2:1); // continue dfs
     }
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
               a[a2][b2] = 1;
               a[b2][a2] = 1;
               //vizinhos[a][vizinhosC[a]++] = b;
               //vizinhos[b][vizinhosC[b]++] = a;
          }
          bipartite = true;
          memset(mark, false, sizeof(bool) * n);
          memset(color, 0, sizeof(int) * n);
          for (i = 0; i < n; i++)
               if (mark[i] == false) dfs(i,1);
          for (i = 0; i <= n; i++)
               memset(a[i], 0, sizeof(int) * n);
          if (bipartite == true) printf("NOT SURE\n");
               else printf("NO\n");    
     }
     
     return 0;
}
