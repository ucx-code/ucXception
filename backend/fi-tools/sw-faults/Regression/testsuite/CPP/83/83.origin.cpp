#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int friendship[2000];
int choices[2000][3];
int afectacoes[2000][200];
int afectacoesCounter[200];

void showC(int * c, int end)
{
     int i;
     for (i = 0; i < end; i++) printf("%d", c[i]);
     printf("\n");
}

int n, m;
int best = 9999999;
int jk[2000];

void preJK(int * c)
{
     int i,a;
     for (i = 0; i < m; i++)
     {
          // Ve se a pessoa e favoravel ou nao
          bool favoravel = false;
          for (a = 0; a < 3; a++)
          {
               int absoluto = abs(choices[i][a])-1;
               if (choices[i][a] == 0)
                    break;
               if ((choices[i][a] < 0) && (c[absoluto] == 0))
               {
                    favoravel = true;
                    break;
               }
          }
          if (favoravel == false)
          {
               jk[i] = friendship[i];
          } 
     }
}

void upJK(int * c, int pos, int end)
{
     int i,j,a; int soma = 0;
     
     for (j = 0; j < afectacoesCounter[pos]; j++)
     {
          i = afectacoes[pos][j];
          // Ve se a pessoa e favoravel ou nao
          bool favoravel = false;
          for (a = 0; a < 3; a++)
          {
               int absoluto = abs(choices[i][a])-1;
               if (choices[i][a] == 0)
                    break;
               if ((choices[i][a] > 0) && (c[absoluto] == 1))
               {
                    favoravel = true;
                    break;
               }
               if ((choices[i][a] < 0) && (c[absoluto] == 0))
               {
                    favoravel = true;
                    break;
               }
          }
          if (favoravel == false)
          {
               jk[i] = friendship[i];
          } else jk[i] = 0;
     }
}

int calc(int * c)
{     
     int i, soma = 0;
     for (i = 0; i < m; i++)
          soma += jk[i];
     //printf("%d\n", soma);
     return soma;
}

void rec(int * c, int begin, int end)
{
     int i;
     
     //showC(c, end);
     int soma = calc(c);
     if ((c[0] == 1) && (c[1] == 0) && (c[2] == 1) && (c[3] == 1))
     {
          showC(c, end);
          printf("%d\n", soma);
     }
  
     if (soma < best) { best = soma; }
     
     int i; 
     
     for (i = begin+1; i < end; i++)
     {
          c[i] = 1;
          upJK(c,i,end);
          rec(c, i, end);
          c[i] = 0;
          upJK(c,i,end);
     }
}
int c[200];
int main()
{

     int i;
     scanf("%d %d", &n, &m);
     for (i = 0; i < m; i++)
     {
          scanf("%d", &friendship[i]);
          int tmp;
          scanf("%d", &tmp); int a = 0;
          while (tmp != 0) { 
               choices[i][a++] = tmp; 
               int val = abs(tmp)-1;
               afectacoes[val][afectacoesCounter[val]++] = i;
               scanf("%d", &tmp); 
 
          }
     }
     preJK(c);
     rec(c, -1, n);   
     printf("%d\n", best);
}
