/**
  * Celso Mendes 2009109378
  * Saul Santos 2009118728
  */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 11

int perfect(char word[MAX]);

int main()
{
    char word[MAX];
    int errpos, i, length;
    while(scanf("%s", word) != EOF){
        length = strlen(word);
        errpos = perfect(word);
        while (errpos<length){
            /*O ciclo for serve para remover o caractér que falha uma das condições*/
            for(i = errpos; i < length; i++){
                word[i] = word[i+1];
            }
            errpos = perfect(word);
            length = strlen(word);
        }
        printf("%s\n",word);
    }

    return 0;
}


/*Esta função vai devolver o tamanho da string se esta for perfeita,
  ou a posição do caractér mais a esquerda que desrespeita uma das condições, caso a palavra não seja perfeita.*/

int perfect(char word[MAX]){
    int i, length, errpos;
    errpos = length = strlen(word);
    /*O nosso i vai variar de 0<=i<length, logo temos de trocar as condições, a condição que deveria ser para os números pares quando 1<=i<=length
    vai agora ser para os números ímpares e vice-versa.*/

    for (i = 0; i < length; i++){
        /*Uma palavra não é perfeita quando verifica, pelo menos, uma das seguintes condições*/
        if (i%2 == 0){

            /*Verificar esta condição é a mesma coisa que falhar a condição "word[i] <= word[length-i-1]"*/
            if (word[i] > word[length-i-1]){
                if ((length-i-1) < errpos){
                    errpos = (length-i-1);
                }
            }
        }else{

            /*Verificar esta condição é a mesma coisa que falhar a condição "word[i] >= word[length-i-1]"*/
            if (word[i] < word[length-i-1]){
                if (i < errpos){
                    errpos = i;
                }
            }
        }
    }
    return errpos;
}
