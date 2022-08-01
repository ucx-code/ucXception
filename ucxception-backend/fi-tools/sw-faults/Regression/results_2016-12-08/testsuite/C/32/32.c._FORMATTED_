#include <stdio.h>
#include <math.h>
#include <string.h>
char matrix[1024][1025];

void gerarMatrix(short  lvl, short  rot)
{
    short i, aux1, aux2, j;
    if (lvl == 0) {
        matrix[0][0] = '0';
    }else{
        gerarMatrix(lvl - 1, rot);
        aux1 = pow(2, lvl);
        aux2 = pow(2, (lvl - 1));
        if (rot == 0) {
            for (i = 0;i < aux1;i++){
                if (i < aux2) {
                    memcpy(&matrix[i][aux2], &matrix[i][0], sizeof (char) * aux2);
                    matrix[i][aux1] = '\0';
                }else{
                    memcpy(&matrix[i][0], &matrix[i - aux2][0], sizeof (char) * aux1);
                    for (j = aux2;j < aux1;j++){
                        matrix[i][j] = matrix[i][j] ^ 1;
                    }
                    matrix[i][aux1] = '\0';
                }
            }
        }else
            if (rot == 1) {
                for (i = 0;i < aux1;i++){
                    if (i < aux2) {
                        memcpy(&matrix[i][aux2], &matrix[i][0], sizeof (char) * aux2);
                        for (j = aux2;j < aux1;j++){
                            matrix[i][j] = matrix[i][j] ^ 1;
                        }
                        matrix[i][aux1] = '\0';
                    }else{
                        memcpy(&matrix[i][0], &matrix[i - aux2][0], sizeof (char) * aux2);
                        memcpy(&matrix[i][aux2], &matrix[i - aux2][0], sizeof (char) * aux2);
                        matrix[i][aux1] = '\0';
                    }
                }
            }else
                if (rot == 2) {
                    for (i = 0;i < aux1;i++){
                        if (i < aux2) {
                            memcpy(&matrix[i][aux2], &matrix[i][0], sizeof (char) * aux2);
                            for (j = 0;j < aux2;j++){
                                matrix[i][j] = matrix[i][j] ^ 1;
                            }
                            matrix[i][aux1] = '\0';
                        }else{
                            memcpy(&matrix[i][0], &matrix[i - aux2][aux2], sizeof (char) * aux2);
                            memcpy(&matrix[i][aux2], &matrix[i][0], sizeof (char) * aux2);
                            matrix[i][aux1] = '\0';
                        }
                    }
                }else{
                    for (i = 0;i < aux1;i++){
                        if (i < aux2) 
                            memcpy(&matrix[i][aux2], &matrix[i][0], sizeof (char) * aux2);
                        else{
                            memcpy(&matrix[i][0], &matrix[i - aux2][0], sizeof (char) * aux1);
                            for (j = 0;j < aux2;j++){
                                matrix[i][j] = matrix[i][j] ^ 1;
                            }
                            matrix[i][aux1] = '\0';
                        }
                    }
                }
    }
}

int main()
{
    short rot, lvl, size, i;
    while (scanf("%hd %hd", &lvl, &rot) != EOF){
        rot = rot % 4;
        size = pow(2, lvl);
        gerarMatrix(lvl, rot);
        for (i = 0;i < size;i++){
            matrix[i][size] = '\0';
            printf("%s\n", matrix[i]);
        }
    }
    return 0;
}
