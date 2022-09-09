#include <stdio.h>
short sack[301][30000];
short ndep, ects;

int main()
{
    short n = 0, i, k, z, aux1, aux2, aux;
    scanf("%hd %hd", &ndep, &ects);
    for (i = 1;i <= ndep;i++){
        scanf("%hd", &n);
        for (z = 0;z < n;z++){
            scanf("%hd %hd", &aux1, &aux2);
            if (z == 0) {
                for (k = 0;k < ects;k++){
                    if (aux1 == (k + 1) && sack[i - 1][k] < aux2) {
                        sack[i][k] = aux2;
                    }else
                        if (aux1 < (k + 1) && sack[i - 1][k] < (aux = (aux2 + sack[i - 1][k - aux1]))) {
                            sack[i][k] = aux;
                        }else{
                            sack[i][k] = sack[i - 1][k];
                        }
                }
            }else{
                for (k = 0;k < ects;k++){
                    if (aux1 == (k + 1) && sack[i][k] < aux2) {
                        sack[i][k] = aux2;
                    }else
                        if (aux1 < (k + 1) && sack[i][k] < (aux = (aux2 + sack[i - 1][k - aux1]))) {
                            sack[i][k] = aux;
                        }
                }
            }
        }
    }
    printf("%hd\n", sack[ndep][ects - 1]);
    return 0;
}
