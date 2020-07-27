/**
  * Celso Mendes 2009109378
  * Saul Santos 2009118728
  */

#include <stdio.h>
#include <stdlib.h>

/*Moedas
    2        moedas[0]
    1        moedas[1]
    0.5      moedas[2]
    0.2      moedas[3]
    0.1      moedas[4]
    0.05     moedas[5]
    0.02     moedas[6]
    0.01     moedas[7]
*/

int main()
{
    int moedas[8], moedasRec[8];
    int moedasTroco[8] = {0,0,0,0,0,0,0,0};
    float valores[8] = {2,1,0.5,0.2,0.1,0.05,0.02,0.01};
    float despesa, recebido=0, troco;
    int i, x;
    /*Entrada do numero de moedas para trocos*/
    for (i=0; i<8; i++){
        scanf("%d", &moedas[i]);
    }

    /*Entrada da despesa*/
    scanf("%f", &despesa);

    /*Entrada das moedas inseridas para pagar a despesa*/
    for (i=0; i<8; i++){
        scanf("%d", &moedasRec[i]);
        recebido = recebido + (moedasRec[i]*valores[i]);
    }


    /*Calculo do troco*/
    if(recebido < despesa){
        printf("Troco: %.2f", despesa);
        for (i=0; i<8; i++){
            printf(" %d", moedas[i]);
        }
        printf(" Valor insuficiente!");
    }else{
        troco = recebido-despesa;

        /*actualiza as moedas existentes na maquina*/
        for (i=0; i<8; i++){
            moedas[i]=moedas[i]+moedasRec[i];
        }

        /*Numero de moedas a devolver*/
        /*Moedas de 2€*/
        if(troco/2>=1 && moedas[0]>0){
            x=troco/2;
            if (x>moedas[0]){
                moedasTroco[0] = moedas[0];
            }else{
                moedasTroco[0] = x;
            }
            moedas[0]=moedas[0]-moedasTroco[0];
            troco = troco - (moedasTroco[0]*2);
        }

        /*Moedas de 1€*/
        if(troco>=1 && moedas[1]>0){
            x=troco;
            if (x>moedas[1]){
                moedasTroco[1] = moedas[1];
            }else{
                moedasTroco[1] = x;
            }
            moedas[1]=moedas[1]-moedasTroco[1];
            troco = troco - moedasTroco[1];
        }

        /*Moedas de 0.5€*/
        if(troco/0.5>=1 && moedas[2]>0){
            x=troco/0.5;
            if (x>moedas[2]){
                moedasTroco[2] = moedas[2];
            }else{
                moedasTroco[2] = x;
            }
            moedas[2]=moedas[2]-moedasTroco[2];
            troco = troco - (moedasTroco[2]*0.5);
        }

        /*Moedas de 0.2€*/
        if(troco/0.2>=1 && moedas[3]>0){
            x=troco/0.2;
            if (x>moedas[3]){
                moedasTroco[3] = moedas[3];
            }else{
                moedasTroco[3] = x;
            }
            moedas[3]=moedas[3]-moedasTroco[3];
            troco = troco - (moedasTroco[3]*0.2);
        }

        /*Moedas de 0.1€*/
        if(troco/0.1>=1 && moedas[4]>0){
            x=troco/0.1;
            if (x>moedas[4]){
                moedasTroco[4] = moedas[4];
            }else{
                moedasTroco[4] = x;
            }
            moedas[4]=moedas[4]-moedasTroco[4];
            troco = troco - (moedasTroco[4]*0.1);
        }

        /*Moedas de 0.05€*/
        if(troco/0.05>=1 && moedas[5]>0){
            x=troco/0.05;
            if (x>moedas[5]){
                moedasTroco[5] = moedas[5];
            }else{
                moedasTroco[5] = x;
            }
            moedas[5]=moedas[5]-moedasTroco[5];
            troco = troco - (moedasTroco[5]*0.05);
        }

        /*Moedas de 0.02€*/
        if(troco/0.02>=1 && moedas[6]>0){
            x=troco/0.02;
            if (x>moedas[6]){
                moedasTroco[6] = moedas[6];
            }else{
                moedasTroco[6] = x;
            }
            moedas[6]=moedas[6]-moedasTroco[6];
            troco = troco - (moedasTroco[6]*0.02);
        }

        /*Moedas de 0.01€*/
        if(troco/0.01>=1 && moedas[7]>0){
            x=troco/0.01;
            if (x>moedas[7]){
                moedasTroco[7] = moedas[7];
            }else{
                moedasTroco[7] = x;
            }
            moedas[7]=moedas[7]-moedasTroco[7];
            troco = troco - (moedasTroco[7]*0.01);
        }

        if ((int)(troco*100)>0){
            printf("Troco: %.2f", recebido);
            for (i=0; i<8; i++){
                printf(" %d", moedasRec[i]);
            }
            printf(" Troco indisponivel!");
        } else{
            printf("Troco: %.2f", (recebido-despesa));
            for (i=0; i<8; i++){
                printf(" %d", moedasTroco[i]);
            }
        }
    }
    printf("\n");

    return 0;
}

