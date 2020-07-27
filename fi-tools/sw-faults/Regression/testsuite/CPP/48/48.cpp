#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUFFER_SIZE 500120
char buffer[BUFFER_SIZE];
char outputBuffer[8000];
int counter = 0;

void strcat2(char* text)
{
    int x = strlen(text);
    memcpy(outputBuffer + counter, text, x);
    counter += x;
    if (counter >= 7000) {
        fwrite(outputBuffer, 1, counter, stdout);
        outputBuffer[0] = '\0';
        counter = 0;
    }
}

void strcat2(char* text, int size)
{
    memcpy(outputBuffer + counter, text, size);
    counter += size;
    if (counter >= 7000) {
        fwrite(outputBuffer, 1, counter, stdout);
        outputBuffer[0] = '\0';
        counter = 0;
    }
}

int getX(int pos, int width)
{
    return pos % (width + 1) + 1;
}

int getY(int pos, int width)
{
    return pos / (width + 1) + 1;
}

char tmp[100];
int recurseSize = 0;

int recurse(int pos, int max, int width)
{
    if ((buffer[pos] & 0x80) == 0x80) 
        return -1;

    buffer[pos] |= 0x80;
    if (pos <= width) {
        sprintf(tmp, "(%d,%d)", getY(pos, width), getX(pos, width));
        strcat2(tmp);
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
                    strcat2((char*)",", 1);

                sprintf(tmp, "(%d,%d)", getY(pos, width), getX(pos, width));
                strcat2(tmp);
                return 1;
            }
        }
        if (bufferPosWidth1 == '|') {
            if (recurse(pos - width - 1, max, width) == 1) {
                if (recurseSize != 0) 
                    strcat2((char*)",", 1);

                sprintf(tmp, "(%d,%d)", getY(pos, width), getX(pos, width));
                strcat2(tmp);
                return 1;
            }
        }
        if (bufferPosWidth2 == '\\') {
            if (recurse(pos - width - 2, max, width) == 1) {
                if (recurseSize != 0) 
                    strcat2((char*)",", 1);

                sprintf(tmp, "(%d,%d)", getY(pos, width), getX(pos, width));
                strcat2(tmp);
                return 1;
            }
        }
    }
    if ((bufferPos == '\\')) {
        if ((bufferPosWidth2 == '|') || (bufferPosWidth2 == '\\')) {
            if (recurse(pos - width - 2, max, width) == 1) {
                if (recurseSize != 0) 
                    strcat2((char*)",", 1);

                sprintf(tmp, "(%d,%d)", getY(pos, width), getX(pos, width));
                strcat2(tmp);
                return 1;
            }
        }
        if (bufferPosWidth1 == '/') {
            if (recurse(pos - width - 1, max, width) == 1) {
                if (recurseSize != 0) 
                    strcat2((char*)",", 1);

                sprintf(tmp, "(%d,%d)", getY(pos, width), getX(pos, width));
                strcat2(tmp);
                return 1;
            }
        }
        if ((bufferPosN1 == '/') && (bufferPosN2 == '\\')) {
            if (recurse(pos - 2, max, width) == 1) {
                if (recurseSize != 0) 
                    strcat2((char*)",", 1);

                sprintf(tmp, "(%d,%d),(%d,%d)", getY(pos - 1, width), getX(pos - 1, width), getY(pos, width), getX(pos, width));
                strcat2(tmp);
                return 1;
            }
        }
    }
    if (bufferPos == '/') {
        if ((bufferPosWidth == '|') || (bufferPosWidth == '/')) {
            if (recurse(pos - width, max, width) == 1) {
                if (recurseSize != 0) 
                    strcat2((char*)",", 1);

                sprintf(tmp, "(%d,%d)", getY(pos, width), getX(pos, width));
                strcat2(tmp);
                return 1;
            }
        }
        if (bufferPosWidth1 == '\\') {
            if (recurse(pos - width - 1, max, width) == 1) {
                if (recurseSize != 0) 
                    strcat2((char*)",", 1);

                sprintf(tmp, "(%d,%d)", getY(pos, width), getX(pos, width));
                strcat2(tmp);
                return 1;
            }
        }
        if ((bufferPos1 == '\\') && (bufferPos2 == '/')) {
            if (recurse(pos + 2, max, width) == 1) {
                if (recurseSize != 0) 
                    strcat2((char*)",", 1);

                sprintf(tmp, "(%d,%d),(%d,%d)", getY(pos + 1, width), getX(pos + 1, width), getY(pos, width), getX(pos, width));
                strcat2(tmp);
                return 1;
            }
        }
    }
    return -1;
}

int main()
{
    outputBuffer[0] = '\0';
    int i, width, height;
    while (true){
        if (scanf("%d %d\n", &height, &width) != 2) 
            break;

        int matrixSize = (width + 1) * height;
        fread(buffer, 1, matrixSize, stdin);
        bool encontrou = false;
        for (i = matrixSize - width - 1;i < matrixSize;i++){
            if (((buffer[i] & 0x7F) == '|') || ((buffer[i] & 0x7F) == '\\') || ((buffer[i] & 0x7F) == '/')) {
                recurseSize = 0;
                int result = recurse(i, matrixSize, width);
                if (result == 1) {
                    strcat2((char*)"\n", 1);
                    encontrou = true;
                    break;
                }
            }
            if (encontrou) 
                break;
        }
        if (!encontrou) 
            strcat2((char*)"No Path!\n", 9);
    }
    outputBuffer[counter] = '\0';
    fwrite(outputBuffer, 1, counter, stdout);
    return 0;
}
