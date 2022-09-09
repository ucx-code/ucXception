#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct _slot { int begin; int end; };
struct _slot in[150000];
struct _slot slot[150000];

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
bool quickSort(int *arr, int elements) {

  #define  MAX_LEVELS  1000

  int  piv, beg[MAX_LEVELS], end[MAX_LEVELS], i=0, L, R ;

  beg[0]=0; end[0]=elements;
  while (i>=0) {
    L=beg[i]; R=end[i]-1;
    if (L<R) {
      piv=arr[L]; if (i==MAX_LEVELS-1) return false;
      while (L<R) {
        while (arr[R]>=piv && L<R) R--; if (L<R) arr[L++]=arr[R];
        while (arr[L]<=piv && L<R) L++; if (L<R) arr[R--]=arr[L]; }
      arr[L]=piv; beg[i+1]=L+1; end[i+1]=end[i]; end[i++]=L; }
    else {
      i--; }}
  return true; }
    
/* Implementacao Heap adaptada de Algorithms in C - Sedgewick */
int N = 1; // N comeca de 1 porque fazemos um insert sem upheap no inicio do for
int a[150000];
inline void upheap(int k)
{
     register int v;
     v = a[k]; a[0] = -999999;
     while (a[k/2] >= v)
     {
          a[k] = a[k/2];
          k = k/2;
     }
     a[k] = v;
}

inline void downheap(int k)
{
     register int j,v;
     v = a[k];
     while (k <= N/2)
     {
          j = k+k;
          if (j < N && a[j]>a[j+1]) j++;
          if (v <= a[j]) break;
          a[k] = a[j]; k = j;
     }
     a[k] = v;
}

void insert(int v)
{
     a[++N] = v;
     upheap(N);
}

void replace(int v)
{
     a[0] = v;
     downheap(0);
}
int naa[150000];
int main()
{
     int n;
     scanf("%d\n", &n);
     int i;
     for (i = 0; i < n; i++)
     {
          scanf("%d %d", &slot[i].begin, &slot[i].end);
          naa[i] = slot[i].begin;
     }
     /* Ordena do que começa mais cedo para o que começa mais tarde */    
    // qsort(slot, n, sizeof(struct _slot), compare);
     // Nao chamamos o insert para evitarmos o upheap, o N comeca a 1 em vez de 0

     //countSort(n);
     //radix_sort(n);
     quickSort(naa, n);   
     //printf("%d\n", (unsigned int)INT_MIN);
     return 0;
     a[1] = slot[0].end;
     
     for (i = 1; i < n; i++)
     {
          if (a[1] <= slot[i].begin)
          {
               replace(slot[i].end); 
          } else {
               insert(slot[i].end); 
          }
          
     }
     printf("%d\n", N);
     return 0;
}
