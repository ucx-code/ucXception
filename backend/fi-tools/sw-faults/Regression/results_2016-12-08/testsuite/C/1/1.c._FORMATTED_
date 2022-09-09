#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
char matrix[100 * 100];

void rec(int n, int target)
{
    if (n > target) 
        return;

    if (n == 0) {
        matrix[0] = '0';
        rec(n + 1, target);
    }else{
        int halfSquare = pow(2, n) / 2;
        int i;
        char tmp[100];
        memcpy(tmp, matrix, halfSquare * halfSquare);
        for (i = 0;i < halfSquare;i++){
            memset(matrix + (i * 2 * halfSquare), '1', halfSquare);
            memcpy(matrix + ((i * 2 + 1) * halfSquare), tmp + halfSquare * i, halfSquare);
        }
        for (i = 0;i < halfSquare;i++){
            memset(matrix + (halfSquare * 2 * halfSquare) + (i * 2 * halfSquare), '2', halfSquare);
            memset(matrix + (halfSquare * 2 * halfSquare) + ((i * 2 + 1) * halfSquare), '3', halfSquare);
        }
        rec(n + 1, target);
    }
}

int main()
{
    int n, r;
    int i;
    scanf("%d %d", &n, &r);
    rec(0, n);
    for (i = 0;i < pow(2, n);i++){
        char tmp[1000];
        strncpy(tmp, matrix + (int)(i * pow(2, n)), pow(2, n));
        tmp[(int)pow(2, n)] = '\0';
        printf("%s\n", tmp);
    }
}
