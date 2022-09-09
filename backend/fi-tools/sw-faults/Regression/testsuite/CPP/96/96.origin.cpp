#include <cstdio>
#include <vector>
#include <queue>
#include <string.h>

using namespace std;

#define MAX 501

int n, e;
int partition[MAX], visited[MAX];
vector< int > G[MAX];

bool is_bipartite(int start)
{
    int i, u, v;
    queue< int > Q;
    //start = 1; // nodes labeled from 1
    Q.push(start);
    partition[start] = 1; // 1 left, 2 right
    visited[start] = 1; // set gray

    while(!Q.empty())
    {
        u = Q.front(); Q.pop();
        for(i=0; i < G[u].size(); i++)
        {
            v = G[u][i];
            if(partition[u] == partition[v]) return false;
            if(visited[v] == 0)
            {
                visited[v] = 1;
                partition[v] = 3 - partition[u]; // alter 1 and 2
                Q.push(v);
            }
        }
    }
    return true;
}

int main()
{
    int i, u, v;
    while (scanf("%d %d", &n, &e) != EOF)
    {
         for(i=0; i<e; i++)
         {
             scanf("%d %d", &u, &v);
             G[u].push_back(v);
             G[v].push_back(u);
         }
         for (i = 1; i <= n;i++)
         if(!is_bipartite(i)) { printf("NO\n"); goto jump; }
         printf("NOT SURE\n");
         jump:
         for (i = 0; i <= n; i++) G[i].clear();
         memset(partition, 0, sizeof(int) * (n+1));
         memset(visited, 0, sizeof(int) * (n+1));
     }
    return 0;
}
