#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
int w[302][18];
int v[302][18];
int nCoursesDept[302];
register int K[302][30001];

int knapsack3(int n, int m)
{
    int j, i, a;
    for (i = 1;i <= n;i++){
        for (j = 1;j <= m;j++){
            for (a = 1;a <= nCoursesDept[i];a++){
                if (w[i][a] > j) 
                    K[i][j] = K[i - 1][j];
                else{
                    int maximo = K[i][j];
                    if (K[i - 1][j] > maximo) 
                        maximo = K[i - 1][j];

                    int novoValor = K[i - 1][j - w[i][a]] + v[i][a];
                    if (novoValor > maximo) 
                        maximo = novoValor;

                    K[i][j] = maximo;
                }
            }
        }
    }
    return K[n][m];
}

int main()
{
    int i;
    int n;
    int m;
    scanf("%d %d", &n, &m);
    for (i = 1;i <= n;i++){
        scanf("%d", &nCoursesDept[i]);
        int a;
        for (a = 1;a <= nCoursesDept[i];a++){
            scanf("%d %d", &w[i][a], &v[i][a]);
        }
    }
    printf("%d\n", knapsack3(n, m));
}
