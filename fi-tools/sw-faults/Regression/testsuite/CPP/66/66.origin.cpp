#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/fcntl.h>

unsigned int slot[150015*2];
void quick_sort (unsigned int *a, int n) {
    if (n < 2)
        return;
    register unsigned int p = a[n / 2];
    unsigned int *l = a;
    unsigned int *r = a + n - 1;
    while (l <= r) {
        if (*l < p) {
            ++l;
            continue;
        }
        if (*r > p) {
            --r;
            continue; 
        }
        register unsigned int t = *l;
        *l = *r;
        *r = t; 
        
        t = *(l+150010);
        *(l+150010) = *(r+150010);
        *(r+150010) = t;
  
        --r; ++l;
    }
    quick_sort(a, r - a + 1);
    quick_sort(l, a + n - l);
}

/* Implementacao Heap adaptada de Algorithms in C - Sedgewick */

unsigned int N = 1; // N comeca de 1 porque fazemos um insert sem upheap no inicio do for
inline void upheap(unsigned int k)
{
     register unsigned int v;
     v = slot[k]; slot[0] = 0;
     while (slot[k/2] >= v)
     {
          slot[k] = slot[k/2];
          k = k/2;
     }
     slot[k] = v;
}

inline void downheap(unsigned int k)
{
     register unsigned int j,v;
     v = slot[k];
     while (k <= N/2)
     {

          j = k+k;
          if (j < N && slot[j] > slot[j+1]) j++;
          if (v <= slot[j]) break;
          slot[k] = slot[j]; k = j;
     }
     slot[k] = v;
}

void insert(int v)
{
     slot[++N] = v;
     upheap(N);
}

void replace(int v)
{
     slot[0] = v;
     downheap(0);
}

struct stat statbuf;
char * src;
unsigned int n;

void readM()
{
     register char* in=src;
     register int i = 1;
     register unsigned int* out=&n;

     while(*in>47) {
       *out= *out * 10 + *in-48;
       ++in;
     }

     ++in; // New line
     while (*in)
     {
          out=&slot[i];

          while(*in>47) {
               *out= *out * 10 + *in-48;
               ++in;
          }

          ++in; // Whitespace
          out = &slot[i+150010];
          while(*in>47) {
               *out= *out* 10 + *in-48;
               ++in;
          }

          ++in; // New line
          ++i;
   }

}
//unsigned int b[150000][2];
int main()
{
     unsigned int i;

     fstat(STDIN_FILENO, &statbuf);

     src = (char *) mmap(0, statbuf.st_size, PROT_READ, MAP_PRIVATE, 0 , 0);
     madvise(src, statbuf.st_size,  MADV_WILLNEED | MADV_SEQUENTIAL);
     readM();
     madvise(src, statbuf.st_size, MADV_DONTNEED);

     /* Ordena do que começa mais cedo para o que começa mais tarde */    
     quick_sort(&slot[0], n);

     // Nao chamamos o insert para evitarmos o upheap, o N comeca a 1 em vez de 0
     slot[1] = slot[1+150010];
     
     for (i = 2; i <= n; i++)
     {
          if (slot[1] <= slot[i+150010])
          {
               replace(slot[i+150010]); 
          } else {
               insert(slot[i+150010]); 
          }
          
     }
     printf("%d\n", N);
     return 0;
}
