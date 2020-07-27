/**
  * Celso Mendes 2009109378
  * Saul Santos 2009118728
  */


#include <stdio.h>
#include <stdlib.h>

/*nr jogadores maximo*/
#define MAX_JOGADORES 300

void ad_jogador(int numero, int pos);
int soma_golos(int jogador);
float equilibrio(int jogador, float media);
int jogadores[MAX_JOGADORES][6];

int main()
{
    int n, i=0, j;
    scanf("%d", &n);
    while(n!=(-1)){
        ad_jogador(n, i);
        i++;
        scanf("%d", &n);
    }

    for(j=0; j<i; j++){
        float soma=soma_golos(j);
        float media=(soma/5);
        float eq=equilibrio(j, media);

        printf("%d\n", jogadores[j][0]);
        printf("Media: %g\n", media);
        printf("Equilibrio: %g\n", eq);

        if(media>eq){
            printf("Classificacao: %g\n", media);
        }
        else{
            printf("Classificacao: %g\n", media/2);
        }

    }
    return 0;
}

void ad_jogador(int numero, int pos){
    int j;
    jogadores[pos][0]=numero;

    for(j=1; j<6; j++){
        scanf("%d\n", &jogadores[pos][j]);
    }

}

int soma_golos(int jogador){
    int soma=0, k;
    for(k=1; k<6; k++){
        soma=soma+jogadores[jogador][k];
    }
    return soma;
}


float equilibrio(int jogador,float media){
    float somatorio=0, eq=0, val=0;
    int k;
    for(k=1; k<6; k++){
        val= (jogadores[jogador][k]-media);
        somatorio+=(val*val);
    }
    eq=somatorio/4;
    return eq;
}
