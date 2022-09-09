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
            /*O ciclo for serve para remover o caract�r que falha uma das condi��es*/
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


/*Esta fun��o vai devolver o tamanho da string se esta for perfeita,
  ou a posi��o do caract�r mais a esquerda que desrespeita uma das condi��es, caso a palavra n�o seja perfeita.*/

int perfect(char word[MAX]){
    int i, length, errpos;
    errpos = length = strlen(word);
    /*O nosso i vai variar de 0<=i<length, logo temos de trocar as condi��es, a condi��o que deveria ser para os n�meros pares quando 1<=i<=length
    vai agora ser para os n�meros �mpares e vice-versa.*/

    for (i = 0; i < length; i++){
        /*Uma palavra n�o � perfeita quando verifica, pelo menos, uma das seguintes condi��es*/
        if (i%2 == 0){

            /*Verificar esta condi��o � a mesma coisa que falhar a condi��o "word[i] <= word[length-i-1]"*/
            if (word[i] > word[length-i-1]){
                if ((length-i-1) < errpos){
                    errpos = (length-i-1);
                }
            }
        }else{

            /*Verificar esta condi��o � a mesma coisa que falhar a condi��o "word[i] >= word[length-i-1]"*/
            if (word[i] < word[length-i-1]){
                if (i < errpos){
                    errpos = i;
                }
            }
        }
    }
    return errpos;
}
