#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
unsigned char matrix2[1024 * 1024];
unsigned char tmpInv[1024 * 1024];
unsigned char tmp2[1024 * 1024];
int quickPow[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

void rec(int n, int target, int r)
{
    if (n > target) 
        return;

    if (n == 0) {
        matrix2[0] = '0';
        rec(n + 1, target, r);
    }else{
        int halfSquare = quickPow[n - 1];
        int halfSquare2 = halfSquare * halfSquare;
        int halfMatrix = halfSquare2 * 2;
        int i;
        unsigned char* matrix;
        unsigned char* tmp;
        if (n % 2 != 0) {
            tmp = (unsigned char*)&matrix2;
            matrix = (unsigned char*)&tmp2;
        }else{
            matrix = (unsigned char*)&matrix2;
            tmp = (unsigned char*)&tmp2;
        }
        for (i = 0;i < halfSquare2;i++)
            tmpInv[i] = 97 - tmp[i];
        for (i = 0;i < halfSquare;i++){
            memcpy(matrix + ((i * 2) * halfSquare), tmp + halfSquare * i, halfSquare);
            memcpy(matrix + ((i * 2 + 1) * halfSquare), tmp + halfSquare * i, halfSquare);
        }
        memcpy(matrix + halfMatrix, matrix, halfMatrix);
        for (i = 0;i < halfSquare;i++){
            if (r == 0) 
                memcpy(matrix + halfMatrix + ((i * 2 + 1) * halfSquare), tmpInv + halfSquare * i, halfSquare);
            else
                if (r == 1) 
                    memcpy(matrix + ((i * 2 + 1) * halfSquare), tmpInv + halfSquare * i, halfSquare);
                else
                    if (r == 2) 
                        memcpy(matrix + ((i * 2) * halfSquare), tmpInv + halfSquare * i, halfSquare);
                    else
                        if (r == 3) 
                            memcpy(matrix + halfMatrix + (i * 2 * halfSquare), tmpInv + halfSquare * i, halfSquare);
        }
        rec(n + 1, target, r);
    }
}

int main()
{
    int n, r;
    int i, squared;
    while (scanf("%d %d", &n, &r) != EOF){
        squared = quickPow[n];
        rec(0, n, r % 4);
        for (i = 0;i < squared;i++){
            if (n % 2 == 0) {
                memcpy(tmp2, matrix2 + (i * squared), squared);
                tmp2[squared] = '\0';
                printf("%s\n", tmp2);
            }else{
                memcpy(matrix2, tmp2 + (i * squared), squared);
                matrix2[squared] = '\0';
                printf("%s\n", matrix2);
            }
        }
    }
}
