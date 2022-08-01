#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int w[302][18];
int v[302][18];
int nCoursesDept[302];
int K[305][300100];

int knapsack3(int n, int m)
{
     int j,i,a; int mesmo = 0;
     // Depts
     for (i = 1; i <= n; i++)
     {
          // ECTS
          for (j = 1; j <= m; j++)
          {
               // Cursos
               int melhorValor = K[i-1][j];
               for (a = 1; a <= nCoursesDept[i]; a++)
               {
                    if (w[i][a] <= j) 
                    {
                         if (K[i-1][j] > melhorValor) melhorValor = K[i-1][j];
                         int novoValor = K[i-1][j-w[i][a]] + v[i][a];
                         if (novoValor > melhorValor) melhorValor = novoValor;
                     
                    }
               } 
               K[i][j] = melhorValor;              
          }
     }
     return K[n][m];
}
/*int knapsack2(int n, int m)
{
     int j,i,a;
     int maximoFinal = 0;
     int anterior = 0;
     int agora = 1;
     for (i = 1; i <= n; i++)
     {
          for (a = 1; a <= nCoursesDept[i]; a++)
               for (j = 1; j <= m; j++)
                    if (w[i][a] > j)
                    {
                         //Dont take i
                         int maximo=0;
                         int k;
                         for (k = 1; k <= nCoursesDept[i-1]; k++)
                              if (K[anterior][k][j] > maximo) maximo = K[anterior][k][j];                          
                         if (maximo > maximoFinal) maximoFinal = maximo;   
                         K[agora][a][j] = maximo;
                    } else {
                         // Maximum of dont take
                         int maximo = 0;
                         int k;
                         for (k =1; k<=nCoursesDept[i-1]; k++)
                              if (K[anterior][k][j] > maximo) maximo = K[anterior][k][j];                      
        
                         int maximo2 = std::max(maximo, v[i][a]);   
                                 
                         for (k = 1; k <= nCoursesDept[i-1]; k++)
                              if ((K[anterior][k][j-w[i][a]]+v[i][a]) > maximo2)
                                   maximo2 =   (K[anterior][k][j-w[i][a]]+v[i][a]);   
                    
                         K[agora][a][j] = maximo2;     
                         if (maximo2 > maximoFinal) maximoFinal = maximo2;          
                    }
          // Troca uma linha na tabela
          int tmp = agora;
          agora = anterior;
          anterior = tmp;
     }

     return maximoFinal;
}*/

int main()
{
     int i;
     int n; // Number of departments
     int m; // Maximum ECTS
     scanf("%d %d", &n, &m);
     for (i = 1; i <= n; i++)
     {
          scanf("%d", &nCoursesDept[i]);
          int a;
          for (a = 1; a <= nCoursesDept[i]; a++)
          {
               scanf("%d %d", &w[i][a], &v[i][a]);
          }
     }
     printf("%d\n", knapsack3(n, m));
}

