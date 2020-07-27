#include <stdio.h>
#include <stdlib.h>
#include <string.h>
unsigned char matrix[1024 * 1024];
unsigned char tmp[512 * 512];
unsigned int quickPow[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

void rec(int n, int target, int r)
{
    if (n > target) 
        return;

    if (n == 0) {
        matrix[0] = '0';
        rec(n + 1, target, r);
    }else{
        int halfSquare = quickPow[n - 1];
        int halfSquare2 = halfSquare * halfSquare;
        int halfMatrix = halfSquare2 * 2;
        register int i;
        memcpy(tmp, matrix, halfSquare2);
        for (i = 0;i < halfSquare;i++){
            memcpy(matrix + ((i * 2) * halfSquare), tmp + halfSquare * i, halfSquare);
            memcpy(matrix + ((i * 2 + 1) * halfSquare), tmp + halfSquare * i, halfSquare);
        }
        memcpy(matrix + halfMatrix, matrix, halfMatrix);
        for (i = 0;i < halfSquare2;i += 4){
            register unsigned int* pont = (unsigned int*)(tmp + i);
            *pont = *pont ^ 0x01010101;
        }
        for (i = 0;i < halfSquare;i++){
            switch (r){
                case 0:
                    memcpy(matrix + halfMatrix + ((i * 2 + 1) * halfSquare), tmp + halfSquare * i, halfSquare);
                    break;
                case 1:
                    memcpy(matrix + ((i * 2 + 1) * halfSquare), tmp + halfSquare * i, halfSquare);
                    break;
                case 2:
                    memcpy(matrix + ((i * 2) * halfSquare), tmp + halfSquare * i, halfSquare);
                    break;
                case 3:
                    memcpy(matrix + halfMatrix + (i * 2 * halfSquare), tmp + halfSquare * i, halfSquare);
                    break;
            }
        }
        rec(n + 1, target, r);
    }
}

int main()
{
    unsigned int n, r;
    register unsigned int i, squared;
    while (scanf("%d %d", &n, &r) != EOF){
        squared = quickPow[n];
        rec(0, n, r % 4);
        for (i = 0;i < squared;i++){
            fwrite(matrix + (squared * i), 1, squared, stdout);
            putchar('\n');
        }
    }
}
