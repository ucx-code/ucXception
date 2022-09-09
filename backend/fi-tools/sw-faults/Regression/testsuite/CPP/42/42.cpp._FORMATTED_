#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int w[302][18];
int v[302][18];
int nCoursesDept[302];
int K[2][18][30001];
int maximo[30001];

int knapsack2(int n, int m)
{
    int j, i, a, k;
    int maximoFinal = 0;
    int anterior = 0;
    int agora = 1;
    for (i = 1;i <= n;i++){
        int numberOfCourses = nCoursesDept[i];
        int numberOfCoursesM1 = nCoursesDept[i - 1];
        for (a = 1;a <= numberOfCourses;a++){
            for (j = 1;j <= m;j++)
                for (k = 1;k <= numberOfCoursesM1;k++)
                    if (K[anterior][k][j] > maximo[j]) 
                        maximo[j] = K[anterior][k][j];
            for (j = 1;j <= m;j++){
                if (w[i][a] > j) {
                    K[agora][a][j] = maximo[j];
                }else{
                    int maximo2 = (maximo[j] > v[i][a]) ? maximo[j] : v[i][a];
                    for (k = 1;k <= numberOfCoursesM1;k++){
                        int tmp = K[anterior][k][j - w[i][a]] + v[i][a];
                        if (tmp > maximo2) 
                            maximo2 = tmp;
                    }
                    K[agora][a][j] = maximo2;
                    if (maximo2 > maximoFinal) 
                        maximoFinal = maximo2;
                }
            }
        }
        int tmp = agora;
        agora = anterior;
        anterior = tmp;
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
