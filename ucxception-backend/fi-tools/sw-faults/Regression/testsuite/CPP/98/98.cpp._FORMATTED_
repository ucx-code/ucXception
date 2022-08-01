#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <queue>
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

std::stack<int> queue;

int bfs(int pos, int max, int width)
{
    queue.push(pos);
    while (!queue.empty()){
        pos = queue.top();
        queue.pop();
        if (pos <= width) {
            printf("%d,", pos);
            while (!queue.empty()){
                int val = queue.top();
                queue.pop();
                printf("%d,", val);
            }
            printf("\n");
            return 1;
        }
        if ((buffer[pos] & 0x80) == 0x80) 
            continue;

        buffer[pos] |= 0x80;
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
                queue.push(pos - width);
            }
            if (bufferPosWidth1 == '|') {
                queue.push(pos - width - 1);
            }
            if (bufferPosWidth2 == '\\') {
                queue.push(pos - width - 2);
            }
        }
        if ((bufferPos == '\\')) {
            if ((bufferPosWidth2 == '|') || (bufferPosWidth2 == '\\')) {
                queue.push(pos - width - 2);
            }
            if (bufferPosWidth1 == '/') {
                queue.push(pos - width - 1);
            }
            if ((bufferPosN1 == '/') && (bufferPosN2 == '\\')) {
                queue.push(pos - 2);
            }
        }
        if (bufferPos == '/') {
            if ((bufferPosWidth == '|') || (bufferPosWidth == '/')) {
                queue.push(pos - width);
            }
            if (bufferPosWidth1 == '\\') {
                queue.push(pos - width - 1);
            }
            if ((bufferPos1 == '\\') && (bufferPos2 == '/')) {
                queue.push(pos + 2);
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
        fread(buffer, 1, matrixSize, stdin);
        bool encontrou = false;
        for (i = matrixSize - width - 1;i < matrixSize;i++){
            if (((buffer[i] & 0x7F) == '|') || ((buffer[i] & 0x7F) == '\\') || ((buffer[i] & 0x7F) == '/')) {
                int result = bfs(i, matrixSize, width);
                if (result == 1) {
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
