#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int w[302][18];
int v[302][18];
int nCoursesDept[302];
int K[305][300100];

int knapsack3(int n, int m)
{
    int j, i, a;
    int mesmo = 0;
    for (i = 1;i <= n;i++){
        for (j = 1;j <= m;j++){
            int melhorValor = K[i - 1][j];
            for (a = 1;a <= nCoursesDept[i];a++){
                if (w[i][a] <= j) {
                    if (K[i - 1][j] > melhorValor) 
                        melhorValor = K[i - 1][j];

                    int novoValor = K[i - 1][j - w[i][a]] + v[i][a];
                    if (novoValor > melhorValor) 
                        melhorValor = novoValor;
                }
            }
            K[i][j] = melhorValor;
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
