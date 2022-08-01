#include <stdio.h>
#include <stdlib.h>

struct _slot { int begin; int end; };
struct _slot slot[150001];

int compare (const void * a, const void * b)
{
  return ( ((struct _slot*)a)->begin - ((struct _slot*)b)->begin );
}

/*
     B1 -> 7.863 O(n*d)
        -> 0.195 O(n * log(d)) (STL)
        -> 0.094 HandCoded Heap
     B2 -> 29.192
        -> 0.116
        -> 0.083
     B3 ->
        ->
        -> 0.055
*/

/* Implementacao Heap adaptada de Algorithms in C - Sedgewick */
int N = 1; // N comeca de 1 porque fazemos um insert sem upheap no inicio do for
inline void upheap(int k)
{
     register int v;
     v = slot[k].begin; slot[0].begin = -999999;
     while (slot[k/2].begin >= v)
     {
          slot[k].begin = slot[k/2].begin;
          k = k/2;
     }
     slot[k].begin = v;
}

inline void downheap(int k)
{
     register int j,v;
     v = slot[k].begin;
     while (k <= N/2)
     {

          j = k+k;
          if (j < N && slot[j].begin>slot[j+1].begin) j++;
          if (v <= slot[j].begin) break;
          slot[k].begin = slot[j].begin; k = j;
     }
     slot[k].begin = v;
}

void insert(int v)
{
     slot[++N].begin = v;
     upheap(N);
}

void replace(int v)
{
     slot[0].begin = v;
     downheap(0);
}

int main()
{
     int n;
     scanf("%d\n", &n);
     int i;
     for (i = 1; i <= n; i++)
     {
          scanf("%d %d", &slot[i].begin, &slot[i].end);
     
     }
     /* Ordena do que começa mais cedo para o que começa mais tarde */    
     qsort(&slot[1], n, sizeof(struct _slot), compare);
     // Nao chamamos o insert para evitarmos o upheap, o N comeca a 1 em vez de 0
     slot[1].begin = slot[1].end;
     
     for (i = 2; i <= n; i++)
     {
          if (slot[1].begin <= slot[i].begin)
          {
               replace(slot[i].end); 
          } else {
               insert(slot[i].end); 
          }
          
     }
     printf("%d\n", N);
     return 0;
}
