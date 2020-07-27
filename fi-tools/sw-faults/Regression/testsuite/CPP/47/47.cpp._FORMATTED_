#include <stdio.h>
#include <stdlib.h>
unsigned short w[301][16];
unsigned short v[301][16];
unsigned short nCoursesDept[301];

unsigned int knapsack3(register unsigned int n, register unsigned int m)
{
    unsigned int K[2][30001];
    register unsigned int j, i;
    unsigned int a;
    unsigned int agora = 1, before = 0;
    register unsigned int novoValor, melhorValor;
    for (i = 1;i <= n;i++){
        for (j = 1;j <= m;j++){
            melhorValor = K[before][j];
            register unsigned int kk = nCoursesDept[i];
            for (a = 1;a <= kk;a++){
                register unsigned int la = w[i][a];
                if (la <= j) {
                    novoValor = K[before][j - w[i][a]] + v[i][a];
                    if (novoValor > melhorValor) 
                        melhorValor = novoValor;
                }
            }
            K[agora][j] = melhorValor;
        }
        agora = agora ^ before;
        before = agora ^ before;
        agora = agora ^ before;
    }
    return K[before][m];
}

int main()
{
    unsigned short i;
    unsigned short n;
    unsigned short m;
    scanf("%hu %hu", &n, &m);
    for (i = 1;i <= n;i++){
        unsigned short a;
        scanf("%hu", &nCoursesDept[i]);
        for (a = 1;a <= nCoursesDept[i];a++){
            scanf("%hu %hu", &w[i][a], &v[i][a]);
        }
    }
    printf("%hu\n", knapsack3(n, m));
}
