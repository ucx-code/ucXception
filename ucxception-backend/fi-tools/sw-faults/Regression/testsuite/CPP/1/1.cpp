#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stack>
#define BUFFER_SIZE 10120
char buffer[BUFFER_SIZE];
char hitCount[BUFFER_SIZE];

int getX(int pos, int width)
{
    return pos % (width + 1) + 1;
}

int getY(int pos, int width)
{
    return pos / (width + 1) + 1;
}

std::stack<int> answer;

int recurse(int pos, int max, int width)
{
    if (hitCount[pos] > 0) 
        return -1;

    ++hitCount[pos];
    if ((pos >= max - width - 1) && (pos < max)) {
        answer.push(pos);
        return 1;
    }
    if (((buffer[pos] == '/') && (buffer[pos + width] == '|')) || ((buffer[pos] == '/') && (buffer[pos + width] == '/')) || ((buffer[pos] == '|') && (buffer[pos + width] == '/'))) {
        if (recurse(pos + width, max, width) == 1) {
            answer.push(pos);
            return 1;
        }
    }
    if (((buffer[pos] == '|') && (buffer[pos + width + 1] == '|')) || ((buffer[pos] == '\\') && (buffer[pos + width + 1] == '/')) || ((buffer[pos] == '/') && (buffer[pos + width + 1] == '\\'))) {
        if (recurse(pos + width + 1, max, width) == 1) {
            answer.push(pos);
            return 1;
        }
    }
    if (((buffer[pos] == '\\') && (buffer[pos + width + 2] == '|')) || ((buffer[pos] == '\\') && (buffer[pos + width + 2] == '\\')) || ((buffer[pos] == '|') && (buffer[pos + width + 2] == '\\'))) {
        if (recurse(pos + width + 2, max, width) == 1) {
            answer.push(pos);
            return 1;
        }
    }
    if (buffer[pos] == '\\') {
        if ((buffer[pos + 1] == '/') && (buffer[pos + 2] == '\\')) {
            if (recurse(pos + 2, max, width) == 1) {
                answer.push(pos + 1);
                answer.push(pos);
                return 1;
            }
        }
    }
    if (buffer[pos] == '/') {
        if ((buffer[pos - 1] == '\\') && (buffer[pos - 2] == '/')) {
            if (recurse(pos - 2, max, width) == 1) {
                answer.push(pos - 1);
                answer.push(pos);
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
        memset(hitCount, 0, matrixSize);
        bool encontrou = false;
        for (i = 0;i < width;i++){
            if (converter(buffer[i]) == '!') {
                int result = recurse(i, matrixSize, width);
                if (result == 1) {
                    int tamanhoCaminho = answer.size();
                    int l;
                    for (l = 0;l < tamanhoCaminho;l++){
                        if (l != 0) 
                            printf(",");

                        int posTmp = answer.top();
                        printf("(%d,%d)", getY(posTmp, width), getX(posTmp, width));
                        answer.pop();
                    }
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
