#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int clausulasUnitarias[2000][2][200];
int counterClausulasUnitarias[2000][2][200];
int n,m;

int amizade[2000];
int escolhas[2000][3];
int counterEscolhas[2000];
int best=99999;

int combinacao[200];
int total(int inicio, int fim)
{
     register int a,i;
     unsigned char estado[2000];
     // Mete a zero
     memset(estado, 0, sizeof(unsigned char) * m);

     for (a = 0; a < m; a++)
     {
          // Ve se esta pessoa quer este ingrediente
          for (i = 0; i < counterEscolhas[a]; i++)
          {     
               if (abs(escolhas[a][i])-1 > fim) 
                         estado[a] = 1; 
               else if (escolhas[a][i] > 0) 
               {
                    if (combinacao[escolhas[a][i]-1] == 1) 
                         estado[a] = 1;
               } else {
                    if (combinacao[abs(escolhas[a][i])-1] == 0)
                         estado[a] = 1;
               }
          }
     }     
     

     int soma = 0;
     for (i = 0; i < m; i++)
          if (estado[i] == 0) soma += amizade[i];
     return soma;
}
void recursivo(int ingrediente)
{
     if (ingrediente == n)
     {
          int t = total(0,n);
          if (t < best) best = t;
          return;
     } 
     if ((ingrediente > 2) && (total(0,ingrediente-1) > best)) return; 
     combinacao[ingrediente] = 1;
     recursivo(ingrediente+1);
     combinacao[ingrediente] = 0;
     recursivo(ingrediente+1);
}
int main()
{
     scanf("%d %d", &n, &m);
     int i;
     for (i = 0; i < m; i++)
     {
          scanf("%d", &amizade[i]);
          int conta;
          for (conta = 0; conta < 4; conta++)
          {
               scanf("%d", &escolhas[i][conta]);
               if (escolhas[i][conta] == 0) break;
          }
          counterEscolhas[i]=conta;
     }
     recursivo(0);
     printf("%d\n", best);
     return 0;
     
}
