#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
unsigned char matrix[1024 * 1024];
unsigned char tmp[512 * 512];
unsigned int i, ia;
unsigned int quickPow[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65535, 131072, 262144};

void rec(int n, int target, int r)
{
    if (n > target) 
        return;

    if (n == 0) {
        matrix[0] = '0';
        rec(n + 1, target, r);
    }else{
        unsigned int halfSquare = quickPow[n - 1];
        unsigned int halfSquare2 = quickPow[(n - 1) * 2];
        unsigned int halfMatrix = halfSquare2 * 2;
        memcpy(tmp, matrix, halfSquare2);
        for (i = 0;i < halfSquare;i++){
            memcpy(matrix + ((i * 2) * halfSquare), tmp + halfSquare * i, halfSquare);
            memcpy(matrix + ((i * 2 + 1) * halfSquare), tmp + halfSquare * i, halfSquare);
        }
        memcpy(matrix + halfMatrix, matrix, halfMatrix);
        unsigned long *lel;
        for (i = 0;i < halfSquare2;i += 4){
            lel = (unsigned long *)(tmp + i);
            *lel = *lel ^ 0x01010101;
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
    unsigned int squared;
    while (scanf("%d %d", &n, &r) != EOF){
        squared = quickPow[n];
        rec(0, n, r % 4);
        for (ia = 0;ia < squared;ia++){
            fwrite(matrix + (squared * ia), 1, squared, stdout);
            putchar('\n');
        }
    }
}
