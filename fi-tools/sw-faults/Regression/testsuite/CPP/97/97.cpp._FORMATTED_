#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 10120
int width, height;
unsigned char buffer[BUFFER_SIZE];

inline int getX(int pos)
{
    return pos % (width + 1) + 1;
}

inline int getY(int pos)
{
    return pos / (width + 1) + 1;
}

int recurseSize = 0;

int recurse(int pos)
{
    if (pos <= width) {
        printf("(%d,%d)", getY(pos), getX(pos));
        return 1;
    }
    ++recurseSize;
    buffer[pos] |= 0x80;
    if (buffer[pos] == ('|' | 0x80)) {
        if (buffer[pos - width] == '/') {
            if (recurse(pos - width) == 1) {
                if (recurseSize != 0) 
                    printf(",");

                printf("(%d,%d)", getY(pos), getX(pos));
                return 1;
            }
        }
        if (buffer[pos - width - 1] == '|') {
            if (recurse(pos - width - 1) == 1) {
                if (recurseSize != 0) 
                    printf(",");

                printf("(%d,%d)", getY(pos), getX(pos));
                return 1;
            }
        }
        if (buffer[pos - width - 2] == '\\') {
            if (recurse(pos - width - 2) == 1) {
                if (recurseSize != 0) 
                    printf(",");

                printf("(%d,%d)", getY(pos), getX(pos));
                return 1;
            }
        }
    }
    if ((buffer[pos] == ('\\' | 0x80))) {
        if ((buffer[pos - width - 2] == '|') || (buffer[pos - width - 2] == '\\')) {
            if (recurse(pos - width - 2) == 1) {
                if (recurseSize != 0) 
                    printf(",");

                printf("(%d,%d)", getY(pos), getX(pos));
                return 1;
            }
        }
        if (buffer[pos - width - 1] == '/') {
            if (recurse(pos - width - 1) == 1) {
                if (recurseSize != 0) 
                    printf(",");

                printf("(%d,%d)", getY(pos), getX(pos));
                return 1;
            }
        }
        if ((buffer[pos - 1] == '/') && (buffer[pos - 2] == '\\')) {
            if (recurse(pos - 2) == 1) {
                if (recurseSize != 0) 
                    printf(",");

                printf("(%d,%d),(%d,%d)", getY(pos - 1), getX(pos - 1), getY(pos), getX(pos));
                return 1;
            }
        }
    }
    if (buffer[pos] == ('/' | 0x80)) {
        if ((buffer[pos - width] == '|') || (buffer[pos - width] == '/')) {
            if (recurse(pos - width) == 1) {
                if (recurseSize != 0) 
                    printf(",");

                printf("(%d,%d)", getY(pos), getX(pos));
                return 1;
            }
        }
        if (buffer[pos - width - 1] == '\\') {
            if (recurse(pos - width - 1) == 1) {
                if (recurseSize != 0) 
                    printf(",");

                printf("(%d,%d)", getY(pos), getX(pos));
                return 1;
            }
        }
        if ((buffer[pos + 1] == '\\') && (buffer[pos + 2] == '/')) {
            if (recurse(pos + 2) == 1) {
                if (recurseSize != 0) 
                    printf(",");

                printf("(%d,%d),(%d,%d)", getY(pos + 1), getX(pos + 1), getY(pos), getX(pos));
                return 1;
            }
        }
    }
    return -1;
}

int main()
{
    int i, matrixSize;
    while (true){
        if (scanf("%d %d\n", &height, &width) != 2) 
            return 1;

        matrixSize = (width + 1) * height;
        fread(buffer, 1, matrixSize, stdin);
        for (i = matrixSize - width - 1;i < matrixSize;i++){
            if ((buffer[i] == '|') || (buffer[i] == '\\') || (buffer[i] == '/')) {
                recurseSize = 0;
                if (recurse(i) == 1) 
                    goto terminei_matrix_sucesso;
            }
        }
        printf("No Path!");
        terminei_matrix_sucesso:
        printf("\n");
    }
    return 0;
}
