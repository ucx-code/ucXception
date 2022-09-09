#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
char matrix[1024 * 1024];
char tmpInv[1024 * 1024];
char tmp[1024 * 1024];
int quickPow[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

void rec0(int n, int target)
{
    if (n > target) 
        return;

    if (n == 0) {
        matrix[0] = '0';
        rec0(n + 1, target);
    }else{
        int halfSquare = quickPow[n - 1];
        int i;
        memcpy(tmp, matrix, halfSquare * halfSquare);
        for (i = 0;i < halfSquare * halfSquare;i++)
            tmpInv[i] = 97 - tmp[i];
        for (i = 0;i < halfSquare;i++){
            memcpy(matrix + ((i * 2) * halfSquare), tmp + halfSquare * i, halfSquare);
            memcpy(matrix + ((i * 2 + 1) * halfSquare), tmp + halfSquare * i, halfSquare);
        }
        memcpy(matrix + (halfSquare * 2 * halfSquare), matrix, halfSquare * 2 * halfSquare);
        for (i = 0;i < halfSquare;i++){
            memcpy(matrix + (halfSquare * 2 * halfSquare) + ((i * 2 + 1) * halfSquare), tmpInv + halfSquare * i, halfSquare);
        }
        rec0(n + 1, target);
    }
}

void rec1(int n, int target)
{
    if (n > target) 
        return;

    if (n == 0) {
        matrix[0] = '0';
        rec1(n + 1, target);
    }else{
        int halfSquare = quickPow[n - 1];
        int i;
        memcpy(tmp, matrix, halfSquare * halfSquare);
        for (i = 0;i < halfSquare * halfSquare;i++)
            tmpInv[i] = 97 - tmp[i];
        for (i = 0;i < halfSquare;i++){
            memcpy(matrix + ((i * 2) * halfSquare), tmp + halfSquare * i, halfSquare);
            memcpy(matrix + ((i * 2 + 1) * halfSquare), tmp + halfSquare * i, halfSquare);
        }
        memcpy(matrix + (halfSquare * 2 * halfSquare), matrix, halfSquare * 2 * halfSquare);
        for (i = 0;i < halfSquare;i++){
            memcpy(matrix + ((i * 2 + 1) * halfSquare), tmpInv + halfSquare * i, halfSquare);
        }
        rec1(n + 1, target);
    }
}

void rec2(int n, int target)
{
    if (n > target) 
        return;

    if (n == 0) {
        matrix[0] = '0';
        rec2(n + 1, target);
    }else{
        int halfSquare = quickPow[n - 1];
        int i;
        memcpy(tmp, matrix, halfSquare * halfSquare);
        for (i = 0;i < halfSquare * halfSquare;i++)
            tmpInv[i] = 97 - tmp[i];
        for (i = 0;i < halfSquare;i++){
            memcpy(matrix + ((i * 2) * halfSquare), tmp + halfSquare * i, halfSquare);
            memcpy(matrix + ((i * 2 + 1) * halfSquare), tmp + halfSquare * i, halfSquare);
        }
        memcpy(matrix + (halfSquare * 2 * halfSquare), matrix, halfSquare * 2 * halfSquare);
        for (i = 0;i < halfSquare;i++){
            memcpy(matrix + ((i * 2) * halfSquare), tmpInv + halfSquare * i, halfSquare);
        }
        rec2(n + 1, target);
    }
}

void rec3(int n, int target)
{
    if (n > target) 
        return;

    if (n == 0) {
        matrix[0] = '0';
        rec3(n + 1, target);
    }else{
        int halfSquare = quickPow[n - 1];
        int i;
        memcpy(tmp, matrix, halfSquare * halfSquare);
        for (i = 0;i < halfSquare * halfSquare;i++)
            tmpInv[i] = 97 - tmp[i];
        for (i = 0;i < halfSquare;i++){
            memcpy(matrix + ((i * 2) * halfSquare), tmp + halfSquare * i, halfSquare);
            memcpy(matrix + ((i * 2 + 1) * halfSquare), tmp + halfSquare * i, halfSquare);
        }
        memcpy(matrix + (halfSquare * 2 * halfSquare), matrix, halfSquare * 2 * halfSquare);
        for (i = 0;i < halfSquare;i++){
            memcpy(matrix + (halfSquare * 2 * halfSquare) + (i * 2 * halfSquare), tmpInv + halfSquare * i, halfSquare);
        }
        rec3(n + 1, target);
    }
}

int main()
{
    int n, r;
    int i, squared;
    while (scanf("%d %d", &n, &r) != EOF){
        squared = quickPow[n];
        switch (r % 4){
            case 0:
                rec0(0, n);
                break;
            case 1:
                rec1(0, n);
                break;
            case 2:
                rec2(0, n);
                break;
            case 3:
                rec3(0, n);
                break;
        }
        for (i = 0;i < squared;i++){
            memcpy(tmp, matrix + (i * squared), squared);
            tmp[squared] = '\0';
            printf("%s\n", tmp);
        }
    }
}
