#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
char matrix[1024 * 1024];
char tmpInv[1024 * 1024];
char tmp[1024 * 1024];

void rec(int n, int target, int r)
{
    if (n == target) 
        return;

    if (n == 0) {
        matrix[0] = '0';
        rec(n + 1, target, r);
    }else{
        int halfSquare = (int)pow(2, n) / 2;
        int i;
        memcpy(tmp, matrix, halfSquare * halfSquare);
        for (i = 0;i < halfSquare * halfSquare;i++)
            tmpInv[i] = (tmp[i] == '0') ? '1' : '0';
        for (i = 0;i < halfSquare;i++){
            if (r == 2) 
                memcpy(matrix + ((i * 2) * halfSquare), tmpInv + halfSquare * i, halfSquare);
            else
                memcpy(matrix + ((i * 2) * halfSquare), tmp + halfSquare * i, halfSquare);

            if (r == 1) 
                memcpy(matrix + ((i * 2 + 1) * halfSquare), tmpInv + halfSquare * i, halfSquare);
            else
                memcpy(matrix + ((i * 2 + 1) * halfSquare), tmp + halfSquare * i, halfSquare);
        }
        for (i = 0;i < halfSquare;i++){
            if (r == 3) 
                memcpy(matrix + (halfSquare * 2 * halfSquare) + (i * 2 * halfSquare), tmpInv + halfSquare * i, halfSquare);
            else
                memcpy(matrix + (halfSquare * 2 * halfSquare) + (i * 2 * halfSquare), tmp + halfSquare * i, halfSquare);

            if (r == 0) {
                memcpy(matrix + (halfSquare * 2 * halfSquare) + ((i * 2 + 1) * halfSquare), tmpInv + halfSquare * i, halfSquare);
            }else{
                memcpy(matrix + (halfSquare * 2 * halfSquare) + ((i * 2 + 1) * halfSquare), tmp + halfSquare * i, halfSquare);
            }
        }
        rec(n + 1, target, r);
    }
}

int main()
{
    int n, r;
    int i;
    while (scanf("%d %d", &n, &r) != EOF){
        int squared = pow(2, n);
        rec(0, n, r % 4);
        for (i = 0;i < squared;i++){
            memcpy(tmp, matrix + (i * squared), squared);
            tmp[squared] = '\0';
            printf("%s\n", tmp);
            printf("%s\n", tmp);
        }
    }
}
