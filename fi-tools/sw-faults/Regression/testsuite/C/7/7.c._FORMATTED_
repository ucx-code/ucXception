#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 11
int perfect(char word[MAX]);

int main()
{
    char word[MAX];
    int errpos, i, length;
    while (scanf("%s", word) != EOF){
        length = strlen(word);
        errpos = perfect(word);
        while (errpos < length){
            for (i = errpos;i < length;i++){
                word[i] = word[i + 1];
            }
            errpos = perfect(word);
            length = strlen(word);
        }
        printf("%s\n", word);
    }
    return 0;
}

int perfect(char word[MAX])
{
    int i, length, errpos;
    errpos = length = strlen(word);
    for (i = 0;i < length;i++){
        if (i % 2 == 0) {
            if (word[i] > word[length - i - 1]) {
                if ((length - i - 1) < errpos) {
                    errpos = (length - i - 1);
                }
            }
        }else{
            if (word[i] < word[length - i - 1]) {
                if (i < errpos) {
                    errpos = i;
                }
            }
        }
    }
    return errpos;
}
