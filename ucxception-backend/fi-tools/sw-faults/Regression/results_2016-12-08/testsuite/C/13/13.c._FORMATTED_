#include <stdio.h>
int vec[5000];
int aux[2][5000];
int nSeq;

void seq()
{
    int i, k, m;
    aux[0][0] = 1;
    aux[1][nSeq - 1] = 1;
    for (i = 1;i < nSeq;i++){
        m = 0;
        for (k = 0;k < i;k++){
            if (vec[k] < vec[i] && aux[0][k] > m) 
                m = aux[0][k];
        }
        aux[0][i] = m + 1;
        m = 0;
        for (k = nSeq - 1;k > i;k--){
            if (vec[k] < vec[nSeq - i - 1] && aux[1][k] > m) 
                m = aux[1][k];
        }
        aux[1][nSeq - i - 1] = m + 1;
    }
}

int main()
{
    int n, i, j, m, z;
    while (scanf("%d", &n) != EOF){
        for (i = 0;i < n;i++){
            scanf("%d", &nSeq);
            for (j = 0;j < nSeq;j++){
                scanf("%d", &vec[j]);
            }
            seq();
            m = 0;
            for (j = 0;j < nSeq;j++){
                z = aux[0][j] + aux[1][j] - 1;
                if (z > m) {
                    m = z;
                }
            }
            printf("%d\n", m);
        }
    }
    return 0;
}

