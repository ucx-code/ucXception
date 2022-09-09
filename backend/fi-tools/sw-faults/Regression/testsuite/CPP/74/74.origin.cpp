#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

int friendship[2000];
int choices[2000][3];
int lista[200][2][2000];
int listaC[200][2];

int unitClause[200][2][2000];
int unitClauseC[200][2];

unsigned char forced[200];

void showC(unsigned char * c, int end)
{
     int i;
     for (i = 0; i < end; i++) printf("%d", c[i]);
     printf("\n");
}

int n, m;
int best = 9999999;

int calc(unsigned char *c, int max)
{
     int i, a;
     unsigned char estado[2000];
     memset(estado, 0, sizeof(unsigned char) * m);
          
     for (i = 0; i < max; i++)
     {
          if (c[i] == 0)
               for (a = 0; a < listaC[i][0]; a++)
               {
                         estado[lista[i][0][a]] = 1;
               }
          else
               for (a = 0; a < listaC[i][1]; a++)
               {
                         estado[lista[i][1][a]] = 1;
               }
     }
     int soma = 0;
     for (i = 0; i < m; i++)
     {
          if (estado[i] == 0) soma+= friendship[i];
     }
     return soma;
}
unsigned char preCalculated[200][2000];
/* Pre calcula o estado que os ingredientes a partir de i podem ter */
void precalc(int max)
{
     int i,a,j;
     for (i = 0; i < max; i++)
     {
          for (j = i+1; j < max; j++)
          {
               for (a = 0; a < listaC[j][0]; a++)
               {
                         preCalculated[i][lista[j][0][a]] = 1;
               }
               for (a = 0; a < listaC[j][1]; a++)
               {
                         preCalculated[i][lista[j][1][a]] = 1;
               }
          }
     }
}


int calcB(unsigned char *c, int begin, int max)
{
     register int i,a;
     unsigned char estado[2000];
     //memset(estado, 0, sizeof(unsigned char) * m);
     memcpy(estado, preCalculated[begin-1], sizeof(unsigned char) * m);   
     for (i = 0; i < begin; i++)
     {
          if (c[i] == 0)
               for (a = 0; a < listaC[i][0]; a++)
               {
                         estado[lista[i][0][a]] = 1;
               }
          else
               for (a = 0; a < listaC[i][1]; a++)
               {
                         estado[lista[i][1][a]] = 1;
               }
     }

     int soma = 0;
     for (i = 0; i < m; i++)
     {
          if (estado[i] == 0) soma+= friendship[i];
     }
     return soma;
}

void rec(unsigned char * c, int begin, int end)
{     
  
     if (begin == end) { 
          int soma = calc(c, end);
          if (soma < best) best = soma;
          //showC(c, end); 
          return;
     }

     // Se ja escolhemos o valor de algumas variaveis
     if (forced[begin] != 3)
     {
          //c[begin] = forced[begin];
          rec(c,begin+1,end);
          return;
     }

     int actual = 0; int actual2 = 0;

     c[begin] = 0;
     actual = calcB(c, begin+1, end);
     c[begin] = 1;
     actual2 = calcB(c, begin+1, end);
     
    
     if (actual2 < best) 
     {
          c[begin] = 1;
          rec(c, begin+1, end);
     }    
     if (actual < best)
     {
          c[begin] = 0;
          rec(c, begin+1, end);
     }
}

unsigned char c[200];

int main()
{

     int i,a;
     scanf("%d %d", &n, &m);
     for (i = 0; i < m; i++)
     {
          scanf("%d", &friendship[i]);
          int tmp; int k;
          scanf("%d", &tmp); int a = 0;
          while (tmp != 0) { 
               if (tmp < 0)
                    lista[abs(tmp)-1][0][listaC[abs(tmp)-1][0]++] = i;
               else 
                    lista[abs(tmp)-1][1][listaC[abs(tmp)-1][1]++] = i;
               choices[i][a++] = tmp; 
               k = tmp;
               scanf("%d", &tmp); }
          if (a == 1)
          {
               // Clausula unitaria
               if (k < 0)
               {
                    unitClause[abs(k)-1][0][unitClauseC[abs(k)-1][0]++] = i;              } else {
                    unitClause[abs(k)-1][1][unitClauseC[abs(k)-1][1]++] = i;           
               }
          }       
     }
     
     precalc(n);

     memset(forced, 3, sizeof(unsigned char) * n);
     /* Pure Literal Rule */
     for (i = 0; i < n; i++)
     {
          if (listaC[i][0] == 0)
          {
               forced[i] = 1; c[i] = 1;
          } else if (listaC[i][1] == 0) {
               forced[i] = 0; c[i] = 0;
          }
     }


     /* Dominating Unit Clause */
     for (i = 0; i < n; i++)
     {
          // Peso das clausulas unitarias com valor i
          int k = 0;
          for (a = 0; a < unitClauseC[i][1]; a++)
               k += friendship[unitClause[i][1][a]];

          // Peso das clausulas (1,2,3) com valor -i
          int k2 = 0;
          for (a = 0; a < listaC[i][0]; a++)
               k2 += friendship[lista[i][0][a]];

          if (k >= k2) { c[i] = 1; forced[i] = 1; continue; }
          
          // Peso das clausulas unitarias com valor -i
          k = 0;
          for (a = 0; a < unitClauseC[i][0]; a++)
               k += friendship[unitClause[i][0][a]];

          // Peso das clausulas (1,2,3) com valor i
          k2 = 0;
          for (a = 0; a < listaC[i][1]; a++)
               k2 += friendship[lista[i][1][a]];

          if (k >= k2) { c[i] = 0; forced[i] = 0;  }
     }

     rec(c, 0, n);   
     printf("%d\n", best);
}
