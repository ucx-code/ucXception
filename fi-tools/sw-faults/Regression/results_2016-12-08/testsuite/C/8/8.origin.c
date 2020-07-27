/**
  * Celso Mendes 2009109378
  * Saul Santos 2009118728
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int checkWord(char *word);
char *maxSubString(char *word);

int main()
{
    int nString, i, tam=0;
    char **pWord;
    char *numStrings, *actWord, *finalWord;

    numStrings = (char *)malloc(5*sizeof(char));

    /*Le o numero de strings que vamos ler*/
    scanf("%s", numStrings);
    while(checkWord(numStrings)==1)
        scanf("%s", numStrings);

    /*Convers�o de uma string para inteiro*/
    nString = atoi(numStrings);

    /*Libertamos a mem�ria alocada porque j� n�o � necess�ria*/
    free(numStrings);

    /*Aloca��o de memoria*/
    pWord = (char **)malloc(nString*sizeof(char*));
    finalWord = (char *)malloc(51*sizeof(char));
    actWord = (char *)malloc(51*sizeof(char));

    for (i=0; i < nString; i++)
        pWord[i] = (char *) malloc(51*sizeof(char));

    /*Leitura e valida��o de dados*/
    for (i=0; i < nString; i++)
    {
        scanf("%s", actWord);
        if(checkWord(actWord) == 0){
            strcpy(pWord[i],actWord);
        }else{
            /*Caso o utilizador insira caract�res inv�lidos tem de voltar a inserir aquela string*/
            i--;
        }
    }

    for (i=0; i < nString; i++){
        strcpy(actWord,pWord[i]);
        actWord = maxSubString(actWord);
        if(tam<strlen(actWord)){
            strcpy(finalWord,actWord);
            tam = strlen(finalWord);
        }
    }

    printf("%s\n", finalWord);

    /*Libertar a mem�ria que foi reservada anteriormente*/
    for (i=0; i < nString; i++)
        free(pWord[i]);

    free(actWord);
    free(pWord);
    free(finalWord);

    return 0;
}

char *maxSubString(char *word){
    int i, j;
    /*As var's i e j s�o variaveis de controlo de ciclos*/

    int tam = 0, idxI = 0, tamAux = 0, idxAux = 0;
    /*A var. tam - vai guardar o tamanho da maior substring, a var. idxI - guarda a posi��o onde inicia a substring que nos interessa*/
    /*As var's tamAux e idxAux s�o variaveis auxiliares*/
    /*Caso a string so seja composta por um caract�r a sequ�ncia maior � esse caract�r*/

    if(strlen(word)>1){

        for (i=1; i<strlen(word); i++){
            tamAux++;

            /*Verifica-mos so o caract�r na posi��o i � igual a algum existente num posi��o anterior a i*/
            for (j=idxAux; j<i; j++){

                if(word[i] == word[j]){
                    if (tam<tamAux){
                        tam = tamAux;
                        idxI = idxAux;
                    }
                    tamAux = 1;
                    idxAux = j+1;
                    i = j+2;
                    j = i;

                /*Caso seja o �ltimo caract�r da string*/
                }else if ((i+1)==strlen(word) && (j+1)==i){
                    tamAux++;
                    if (tam<tamAux){

                        tam = tamAux;
                        idxI = idxAux;
                    }
                }
            }
        }

        /*Este ciclo serve para "cortar" a substring da string principal*/
        for(i=0; i<tam; i++)
            word[i]=word[i+idxI];
        word[tam]=0;
    }

    return word;
}

int checkWord(char *word){
    int i, z=0;
    for (i=0; i<strlen(word) && z==0; i++){
        if(word[i] < '0' || word[i] > '9'){
            z = 1;
        }
    }
    return z;
}
