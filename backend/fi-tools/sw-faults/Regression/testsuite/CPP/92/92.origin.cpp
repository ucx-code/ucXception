#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

int vizinhos[501][501];
int vizinhosC[501];
int color[501];

bool dfs(int node, int c) {
   
   if(color[node] == c) {
       return true;
   } else if (color[node] == -c){
       return false;
   }
    color[node] = c;
    for (int i = 0; i < vizinhosC[node]; i++)
          if(dfs(vizinhos[node][i], -c) == false) {
                 return false;
          }
    
    return true;
}

struct stat statbuf;
char * src;
inline void readLine(int * a, int * b)
{
     register char * tmp = src;
     register unsigned int tmpA, tmpB;
     tmpA = (*tmp - 48);
     ++tmp;
     while (*tmp != ' ')
     {
          tmpA = tmpA*10 + (*tmp-48);
          ++tmp;
     }

     ++tmp;
     tmpB = (*tmp - 48);
     ++tmp;
     while (*tmp != '\n')
     {
          tmpB = tmpB*10 + (*tmp-48);
          ++tmp;
     }
     ++tmp;
     src = tmp;
     *a = tmpA; *b = tmpB;
}

int main()
{
     // Le input
     fstat(STDIN_FILENO, &statbuf);
     src = (char *) mmap(0, statbuf.st_size, PROT_READ|PROT_WRITE, MAP_PRIVATE, 0 , 0);
     src[statbuf.st_size] = EOF;

     int n,m; register int i; int it = 0;
     while (true)
     {    
          if (*src == EOF) break;
          readLine(&n, &m);
          for (i = 0; i < m; i++)
          {
               int a, b;
               readLine(&a, &b);
               vizinhos[a][vizinhosC[a]++] = b;
               vizinhos[b][vizinhosC[b]++] = a;
          }  ++it;
          if (dfs(1,it) == true) printf("NOT SURE\n"); else printf("NO\n");
          memset(vizinhosC, 0, sizeof(int) * (n+1));
     }
     
     return 0;
}
