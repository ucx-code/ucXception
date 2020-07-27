#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <list>

struct _slot { int begin; int end; };
struct _slot slot[150000];


bool operator<(const struct _slot& a, const struct _slot& b) {
    return a.end < b.end;
}

int main()
{
     int n;
     scanf("%d\n", &n);
     int i;
     for (i = 0; i < n; i++)
     {
          scanf("%d %d", &slot[i].begin, &slot[i].end);
     }
     /* Ordena do que termina mais cedo para o que termina mais tarde */     
     std::sort(slot, slot+n);

     /* Passa para linked list */
     std::list <struct _slot *> lista;  
     for (i = 0; i < n; i++)
     {
          lista.push_back(&slot[i]);
     }
    
     /* Mostra */
     /*for (i = 0; i < n; i++)
     {
          printf("%d %d %d %d\n", head, head->begin, head->end, head->parent);
          head = head->next;
     }*/
     int total = n;
     int rooms = 0;
     while (true)
     {
          //printf("ROOM\n");
          ++rooms;
          std::list<struct _slot *>::iterator ia = lista.begin();
          std::list<struct _slot *>::iterator master = lista.begin();  
          //int conta = 0;   
          for (; ia != lista.end(); ia++)
          {
                //printf("Master %d %d Actual %d %d\n", (*master)->begin, (*master)->end, (*ia)->begin, (*ia)->end);
               if ((*master)->end <= (*ia)->begin)
               {
                    //++conta;
                    /* Elimina o master */
                    lista.erase(master);
                    master = ia;
               }
          }
          
          if (master == lista.begin())
          {
               //++conta;
               /* Elimina o master */
               lista.erase(master);
               break;
          } else {
              // ++conta;
               /* Elimina o master */
               lista.erase(master);
          }
          //total -= conta -1;
     }
     /*std::list<struct _slot *>::iterator ia = lista.begin();
     for (; ia != lista.end(); ia++)
          printf("LEFT-OVER %d %d\n", (*ia)->begin, (*ia)->end);
     printf("%d ROOMS:%d LEFT-OVER:%d\n", total, rooms, lista.size());*/
     printf("%d\n", rooms+lista.size());
     return 0;
}
