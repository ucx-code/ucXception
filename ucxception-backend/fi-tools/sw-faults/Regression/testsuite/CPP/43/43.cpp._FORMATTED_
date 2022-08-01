#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
int w[100][100];
int v[100][100];
int nCoursesDept[100];
int K[100][100][100];

int knapsack2(int n, int m)
{
    int j, i, a;
    for (j = 1;j <= m;j++)
        K[0][1][j] = 0;
    for (i = 0;i <= n;i++)
        K[i][1][0] = 0;
    for (i = 1;i <= n;i++)
        for (a = 1;a <= nCoursesDept[i];a++)
            for (j = 1;j <= m;j++)
                if (w[i][a] > j) {
                    int maximo = 0;
                    int k;
                    for (k = 1;k <= nCoursesDept[i - 1];k++)
                        if (K[i - 1][k][j] > maximo) 
                            maximo = K[i - 1][k][j];
                    K[i][a][j] = maximo;
                }else{
                    int maximo = 0;
                    int k;
                    for (k = 1;k <= nCoursesDept[i - 1];k++)
                        if (K[i - 1][k][j] > maximo) 
                            maximo = K[i - 1][k][j];
                    int maximo2 = maximo;
                    for (k = 1;k <= nCoursesDept[i - 1];k++)
                        if ((K[i - 1][k][j - w[i][a]] + v[i][a]) > maximo2) 
                            maximo2 = (K[i - 1][k][j - w[i][a]] + v[i][a]);
                    K[i][a][j] = maximo2;
                }
    int k;
    int maximo = 0;
    for (k = 1;k <= nCoursesDept[n];k++)
        if (K[n][k][m] > maximo) 
            maximo = K[n][k][m];
    return maximo;
}

int main()
{
    int n = 4;
    int m = 18;
    nCoursesDept[0] = 0;
    nCoursesDept[1] = 3;
    nCoursesDept[2] = 2;
    nCoursesDept[3] = 2;
    nCoursesDept[4] = 1;
    w[1][1] = 9;
    v[1][1] = 12;
    w[1][2] = 8;
    v[1][2] = 11;
    w[1][3] = 9;
    v[1][3] = 5;
    w[2][1] = 11;
    v[2][1] = 10;
    w[2][2] = 11;
    v[2][2] = 14;
    w[3][1] = 11;
    v[3][1] = 7;
    w[3][2] = 4;
    v[3][2] = 13;
    w[4][1] = 11;
    v[4][1] = 10;
    printf("%d\n", knapsack2(n, m));
}
