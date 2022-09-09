#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stack>
#define BUFFER_SIZE 10120
char buffer[BUFFER_SIZE];

int getX(int pos, int width)
{
    return pos % (width + 1) + 1;
}

int getY(int pos, int width)
{
    return pos / (width + 1) + 1;
}

int recurseSize = 0;

int recurse(int pos, int max, int width)
{
    if ((buffer[pos] & 0x80) == 0x80) 
        return -1;

    buffer[pos] |= 0x80;
    if (pos <= width) {
        printf("(%d,%d)", getY(pos, width), getX(pos, width));
        return 1;
    }
    ++recurseSize;
    int bufferPos = (buffer[pos] & 0x7F);
    int bufferPosWidth = (buffer[pos - width] & 0x7F);
    int bufferPosWidth1 = (buffer[pos - width - 1] & 0x7F);
    int bufferPosWidth2 = (buffer[pos - width - 2] & 0x7F);
    int bufferPos1 = (buffer[pos + 1] & 0x7F);
    int bufferPos2 = (buffer[pos + 2] & 0x7F);
    int bufferPosN1 = (buffer[pos - 1] & 0x7F);
    int bufferPosN2 = (buffer[pos - 2] & 0x7F);
    if (bufferPos == '|') {
        if (bufferPosWidth == '/') {
            if (recurse(pos - width, max, width) == 1) {
                if (recurseSize != 0) 
                    printf(",");

                printf("(%d,%d)", getY(pos, width), getX(pos, width));
                return 1;
            }
        }
        if (bufferPosWidth1 == '|') {
            if (recurse(pos - width - 1, max, width) == 1) {
                if (recurseSize != 0) 
                    printf(",");

                printf("(%d,%d)", getY(pos, width), getX(pos, width));
                return 1;
            }
        }
        if (bufferPosWidth2 == '\\') {
            if (recurse(pos - width - 2, max, width) == 1) {
                if (recurseSize != 0) 
                    printf(",");

                printf("(%d,%d)", getY(pos, width), getX(pos, width));
                return 1;
            }
        }
    }
    if ((bufferPos == '\\')) {
        if ((bufferPosWidth2 == '|') || (bufferPosWidth2 == '\\')) {
            if (recurse(pos - width - 2, max, width) == 1) {
                if (recurseSize != 0) 
                    printf(",");

                printf("(%d,%d)", getY(pos, width), getX(pos, width));
                return 1;
            }
        }
        if (bufferPosWidth1 == '/') {
            if (recurse(pos - width - 1, max, width) == 1) {
                if (recurseSize != 0) 
                    printf(",");

                printf("(%d,%d)", getY(pos, width), getX(pos, width));
                return 1;
            }
        }
        if ((bufferPosN1 == '/') && (bufferPosN2 == '\\')) {
            if (recurse(pos - 2, max, width) == 1) {
                if (recurseSize != 0) 
                    printf(",");

                printf("(%d,%d),", getY(pos - 1, width), getX(pos - 1, width));
                printf("(%d,%d)", getY(pos, width), getX(pos, width));
                return 1;
            }
        }
    }
    if (bufferPos == '/') {
        if ((bufferPosWidth == '|') || (bufferPosWidth == '/')) {
            if (recurse(pos - width, max, width) == 1) {
                if (recurseSize != 0) 
                    printf(",");

                printf("(%d,%d)", getY(pos, width), getX(pos, width));
                return 1;
            }
        }
        if (bufferPosWidth1 == '\\') {
            if (recurse(pos - width - 1, max, width) == 1) {
                if (recurseSize != 0) 
                    printf(",");

                printf("(%d,%d)", getY(pos, width), getX(pos, width));
                return 1;
            }
        }
        if ((bufferPos1 == '\\') && (bufferPos2 == '/')) {
            if (recurse(pos + 2, max, width) == 1) {
                if (recurseSize != 0) 
                    printf(",");

                printf("(%d,%d),", getY(pos + 1, width), getX(pos + 1, width));
                printf("(%d,%d)", getY(pos, width), getX(pos, width));
                return 1;
            }
        }
    }
    return -1;
}

int main()
{
    int i, width, height;
    while (true){
        if (scanf("%d %d\n", &height, &width) != 2) 
            return 0;

        int matrixSize = (width + 1) * height;
        for (i = 0;i < matrixSize;i++){
            buffer[i] = getchar();
        }
        bool encontrou = false;
        for (i = matrixSize - width - 1;i < matrixSize;i++){
            if (((buffer[i] & 0x7F) == '|') || ((buffer[i] & 0x7F) == '\\') || ((buffer[i] & 0x7F) == '/')) {
                recurseSize = 0;
                int result = recurse(i, matrixSize, width);
                if (result == 1) {
                    printf("\n");
                    encontrou = true;
                    break;
                }
            }
            if (encontrou) 
                break;
        }
        if (!encontrou) 
            printf("No Path!\n");
    }
    return 0;
}
