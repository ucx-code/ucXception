#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
int lista[3];
int main()
{
     srand(time(NULL));
     //int n = rand()%15+5; int m = rand()%200+200;
     int n =  50; int m = 500;     
     printf("%d %d\n", n, m);
     int i, a,j;
     for (i = 0; i < m; i++)
     {
          printf("%d", rand() % 99 + 1);
          int max = rand()%2+1;
          memset(lista, 0, sizeof(int)*3);          
          for (a = 0; a < max; a++) {
               bool no = true;
               while (no == true)
               {
                    no = false;
                     j = (rand() % (n)) +1;
                    for (int kk = 0; kk < 3; kk++)
                         if (j == lista[kk]) no = true;
               }
               for (int kk = 0; kk < 3; kk++)
                    if (lista[kk] == 0)
                    {
                         lista[kk] = j; break;
                    }
                         
               if ((rand() % 2) == 0) printf(" %d", j); else printf(" -%d", j);
          }
          printf(" 0\n");
     }
}
