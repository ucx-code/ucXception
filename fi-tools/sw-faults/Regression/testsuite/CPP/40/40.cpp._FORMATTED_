#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
int w[302][18];
int v[302][18];
int nCoursesDept[18];
int K[2][18][30001];

int knapsack2(int n, int m)
{
    int j, i, a;
    int maximoFinal = 0;
    int anterior = 0;
    int agora = 1;
    for (i = 1;i <= n;i++){
        for (a = 1;a <= nCoursesDept[i];a++)
            for (j = 1;j <= m;j++)
                if (w[i][a] > j) {
                    int maximo = 0;
                    int k;
                    for (k = 1;k <= nCoursesDept[i - 1];k++)
                        if (K[anterior][k][j] > maximo) 
                            maximo = K[anterior][k][j];
                    if (maximo > maximoFinal) 
                        maximoFinal = maximo;

                    K[agora][a][j] = maximo;
                }else{
                    int maximo = 0;
                    int k;
                    for (k = 1;k <= nCoursesDept[i - 1];k++)
                        if (K[anterior][k][j] > maximo) 
                            maximo = K[anterior][k][j];
                    int maximo2 = std::max(maximo, v[i][a]);
                    for (k = 1;k <= nCoursesDept[i - 1];k++)
                        if ((K[anterior][k][j - w[i][a]] + v[i][a]) > maximo2) 
                            maximo2 = (K[anterior][k][j - w[i][a]] + v[i][a]);
                    K[agora][a][j] = maximo2;
                    if (maximo2 > maximoFinal) 
                        maximoFinal = maximo2;
                }
        int tmp = agora;
        agora = anterior;
        anterior = agora;
    }
    return maximoFinal;
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
    printf("%d\n", knapsack2(n, m));
}
